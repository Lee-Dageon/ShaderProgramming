#version 330 core

layout(location = 0) 
in vec3 a_Pos;
in vec2 a_Tex;

void main()
{
	gl_Position = vec4(a_Pos, 1.0);
}
