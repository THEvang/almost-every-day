#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "every_math.h"

void
framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void
process_input(GLFWwindow* window) {
	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void*
read_entire_file(const char* file_name) {
	
	const char* mode = "r";
	FILE* file = fopen(file_name, mode);
	if (file == NULL) {
		printf("Failed to open file\n");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long int file_size = ftell(file);
	rewind(file);

	void* content = malloc(file_size);
	if (content == NULL) {
		fclose(file);
		return NULL;
	}
	
	fread(content, 1, file_size, file);
	fclose(file);

	return content;
}

void
log_compile_results(const char* file_name, unsigned int shader) {
	
	int success;
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, info);
		fprintf(stderr, "%s: %s", file_name, info);
	}
}

void
log_link_results(unsigned int shader_program) {
	
	int success;
	char info[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info);
		fprintf(stderr, "%s", info);
	}
}

struct ShaderSources {
	const char* vertex;
	const char* fragment;
	int has_changed;
};

struct ShaderProgram {
	struct ShaderSources sources;
	unsigned int id;
};

struct ShaderProgram
read_and_compile_shaders(struct ShaderSources sources) {

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	void* vertex_shader_source = read_entire_file(sources.vertex);
	glShaderSource(vertex_shader, 1, (const char**) &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	free(vertex_shader_source);
	vertex_shader_source = NULL;

	log_compile_results(sources.vertex, vertex_shader);

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	void* fragment_shader_source = read_entire_file(sources.fragment);
	glShaderSource(fragment_shader, 1, (const char**) &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	free(fragment_shader_source);
	fragment_shader_source = NULL;

	log_compile_results(sources.fragment, fragment_shader);

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	log_link_results(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return (struct ShaderProgram) {sources, shader_program};
}

int
file_changed(const char* path, time_t* old_time) {
	struct stat file_stat;
	int err = stat(path, &file_stat);
	
	if (err != 0) {
		return 0;
	}
	
	if (file_stat.st_mtime > *old_time) {
		*old_time = file_stat.st_mtime;	
		return 1;
	} else {
		return 0;
	}
	
};

int
main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800;
	int height = 600;
	const char* title = "Game";
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		goto TERMINATE;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		goto TERMINATE;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	
	struct ShaderSources shader_sources = {
		"shaders/default.vert",
		"shaders/default.frag",
		0
	};
	struct ShaderProgram shader_program = {0};
	
	time_t old_time_vertex = {0};
	time_t old_time_fragment = {0};

	Vector3 a = {{1, 0, 0}};
	Vector3 b = {{1, 0, 1}};
	Vector3 c = vec3_cross(a,b);

	while(!glfwWindowShouldClose(window)) {
		process_input(window);

		if (file_changed(shader_sources.vertex, &old_time_vertex) || 
				file_changed(shader_sources.fragment, &old_time_fragment)) {
			glDeleteProgram(shader_program.id);
			shader_program = read_and_compile_shaders(shader_sources);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program.id);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

TERMINATE:;
	
	int exit_code = 0;
	const char* description;
	int error_code = glfwGetError(&description);
	if (error_code != GLFW_NO_ERROR) {
		exit_code = -1;
		fprintf(stderr, "%s\n", description);
	}

	glfwTerminate();

	return exit_code;
}
