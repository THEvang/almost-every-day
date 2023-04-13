#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

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

int
read_and_compile_shaders(const char* vertex_shader_file, const char* fragment_shader_file) {

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	void* vertex_shader_source = read_entire_file(vertex_shader_file);
	glShaderSource(vertex_shader, 1, (const char**) &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	free(vertex_shader_source);

	log_compile_results(vertex_shader_file, vertex_shader);

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	void* fragment_shader_source = read_entire_file(fragment_shader_file);
	glShaderSource(fragment_shader, 1, (const char**) &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	free(fragment_shader_source);

	log_compile_results(fragment_shader_file, fragment_shader);

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

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
	
	int shader_program = read_and_compile_shaders("shaders/default.vert", "shaders/default.frag");

	while(!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
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
