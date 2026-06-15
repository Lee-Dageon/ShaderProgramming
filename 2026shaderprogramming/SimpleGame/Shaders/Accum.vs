#version 330

in vec3 a_Pos;
out vec2 v_Tex;
uniform bool u_Flip;

in vec2 a_Tex;

uniform float u_Time;

void main()
{
	vec4 newPosition;
	//newPosition = vec4(a_Pos * u_Trans.w + vec3(u_Trans.xy, 0.0), 1.0);
	newPosition = vec4(a_Pos, 1);

	gl_Position = newPosition;

	v_Tex.x = (a_Pos.x + 1)/2.0;

	if(u_Flip)
	{
		v_Tex.y = 1.0 - (a_Pos.y + 1.0)/2.0;
	}
	else
	{
		v_Tex.y = (a_Pos.y + 1.0)/2.0;
	}
}
