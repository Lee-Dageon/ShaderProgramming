#version 330

in vec3 a_Position;
uniform float u_Time;

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

void main()
{
	sin2();
}
