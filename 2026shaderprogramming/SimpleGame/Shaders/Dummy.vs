#version 330

uniform float u_Time;
in vec3 a_Pos;
const float c_PI = 3.141592;

void main()
{
	float newX = a_Pos.x + 0;
	float newY = a_Pos.y + 0.5 * sin(a_Pos.x * 0.5 + u_Time)*2*c_PI;
	vec4 newPosition = vec4(newX, newY, 0.0, 1.0);
	gl_Position = newPosition;
}
