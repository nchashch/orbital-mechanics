#version 430 core
layout(location = 2) uniform vec4 color;
out vec4 color_output;

void main(void) {
	color_output = color;
}
