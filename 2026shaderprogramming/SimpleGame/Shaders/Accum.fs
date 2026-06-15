#version 330

layout (location=0) out vec4 FragColor;

uniform sampler2D u_Tex;
uniform sampler2D u_TexBlurred;

in vec2 v_Tex;

void main()
{
	float gamma = 2.2;
	vec3 color = texture(u_Tex, v_Tex).rgb;
	vec3 blurred = texture(u_TexBlurred, vec2(v_Tex.x, 1 - v_Tex.y)).rgb;

	vec3 summed = color + blurred;
	FragColor = vec4(summed, 1);
}
