#version 330

layout(location=0) out vec4 FragColor;

in float v_Grey;
in vec3 v_Color;
in vec2 v_Tex;

void main()
{
	float d = distance(vec2(0.5, 0.5), v_Tex);
	if (d < 0.5)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}
}