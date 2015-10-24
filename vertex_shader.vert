#version 430 core

layout(location = 0) uniform mat4 modelview;
layout(location = 1) uniform mat4 projection;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out float intensity;

uniform vec3 lightDirection = vec3(1.0, 0.0, 0.0);

void main() {
	gl_Position = projection * modelview * vec4(vertex, 1.0);
	intensity = dot(lightDirection, normal)*0.6 + 0.1;
}
