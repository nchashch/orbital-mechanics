#version 430 core

layout(location = 0) uniform mat4 modelview;
layout(location = 1) uniform mat4 projection;

layout(location = 0) in vec3 vertex;

void main() {
	gl_Position = projection * modelview * vec4(vertex, 1.0);
}
