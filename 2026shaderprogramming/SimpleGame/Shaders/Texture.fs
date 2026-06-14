#version 330

layout (location=0) out vec4 FragColor;

in vec2 v_Tex;
uniform sampler2D u_Tex;

void main()
{
	FragColor = texture(u_Tex, v_Tex);
}