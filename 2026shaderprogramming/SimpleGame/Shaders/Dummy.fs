#version 330

layout(location=0) out vec4 FragColor;

uniform vec4 u_Color;

in float v_Grey;

void main()
{
	FragColor = vec4(v_Grey);
}
