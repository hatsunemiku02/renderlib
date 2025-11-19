#version 450


layout(location = 0) out vec4 outColor;
layout (location = 1) in vec2 texCoord;
layout(set = 0, binding = 0) uniform sampler2D tex1;

void main() {
	vec3 color = texture(tex1,texCoord).xyz;
    outColor = vec4(color, 1.0f);
}