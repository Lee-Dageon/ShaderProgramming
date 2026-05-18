#version 330

in vec3 a_Pos;

void main()
{
	float newX = a_Pos.x + 0;
	float newY = a_Pos.y + 0.5 * sin(a_Pos.x * 0.5)*2*c_PI;
	vec4 newPosition = vec4(newX, newY, 0.0, 1.0);
	gl_Position = newPosition;
}
