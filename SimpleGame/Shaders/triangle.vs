#version 330

in vec3 a_Position;
uniform float u_Time;
in vec3 a_Mass;
in vec2 a_Vel;

const float c_PI = 3.141592;
const vec2 c_Gravity = vec2(0, -9.8);

// Basic: move right and reset
void Basic()
{	
	float t = mod (u_Time*5, 1.0);	// 0~1
	vec4 newPosition;
	newPosition = vec4(a_Position.x + t, a_Position.y, a_Position.z, 1);

	gl_Position = newPosition;
}

void sin1()
{
	float t = mod (u_Time, 1.0);	// 0~1
	vec4 newPosition;
	newPosition.x = a_Position.x + t;
	newPosition.y = a_Position.y + sin(t*2*3.141592);	// 0~2pi
	newPosition.z = a_Position.z;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void sin2()
{
	float t = mod (u_Time*5, 1.0);	// 0~1
	vec4 newPosition;
	newPosition.x = a_Position.x - 1 + t*2;
	newPosition.y = a_Position.y + sin(t*2*3.141592);	// 0~2pi
	newPosition.z = a_Position.z;
	newPosition.w = 1;
	gl_Position = newPosition;
}

void Circle()
{
	float t = mod (u_Time * 5, 1.0);	// 0~1
	vec4 newPosition;
	newPosition.x = a_Position.x + sin(t*2*3.141592);	// 0~2pi
	newPosition.y = a_Position.y + cos(t*2*3.141592);	// 0~2pi
	newPosition.z = a_Position.z;
	newPosition.w = 1.0f;
	gl_Position = newPosition;
}

void falling()
{
	float t = mod(u_Time, 1.0);	// 0~1
	vec4 newPos;
	newPos.x = a_Position.x;
	newPos.y = a_Position.y + a_Vel.y * t + 0.5 * c_Gravity.y * t * t;
	newPos.z = 0;
	newPos.w = 1;

	gl_Position = newPos;
}

void main()
{
	falling();
}
