#version 330

in vec3 a_Pos;
uniform vec4 u_Trans;

void main()
{
	gl_Position = vec4(a_Pos, 1);
}
