#version 330

layout(location=0) out vec4 FragColor;

in float v_Grey;
in vec3 v_Color;
in vec2 v_Tex;

void main()
{
	// v_Color은 이미 v_Grey로 감쇄된 색상일 수 있으므로 그대로 출력
	FragColor = vec4(v_Color, 1.0);
}