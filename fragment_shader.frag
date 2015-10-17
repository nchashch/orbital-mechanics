#version 430 core
out vec4 color_output;

in vec4 position;

void main(void) {
	vec3 color = vec3(1.0, 1.0, 1.0);
	color /= position.z;
	color_output = vec4(color, 1.0);
}
