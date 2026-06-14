#version 330

uniform float u_Time;
in vec3 a_Pos;
const float c_PI = 3.141592;
out float v_Grey;
out vec2 v_Tex;

void Flag()
{	
	float tX, tY;
	tX = a_Pos.x + 0.5;
	tY = 1.0 - (a_Pos.y + 0.5);
	v_Tex = vec2(tX, tY);

	float value = a_Pos.x + 0.5; // 0~1

	float newX = a_Pos.x + 0;
	float newY = a_Pos.y*(1.0-(value*0.5)) + 
	value * 0.25 * sin((a_Pos.x+0.5)*2*c_PI - u_Time);

	vec4 newPosition = vec4(newX, newY, 0.0, 1.0);

	v_Grey = (1.0+sin((a_Pos.x+0.5)*2*c_PI - u_Time))/2.0;

	gl_Position = newPosition;
	}

void main()
{
	Flag();
}
