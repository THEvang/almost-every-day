#version 330 core

layout (location = 0) in vec3 aPos;

void
main() {
	gl_Position = vec4(aPos.x, aPos.y + 0.99, aPos.z, 1.0);
}

