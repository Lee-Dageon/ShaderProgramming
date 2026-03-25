#version 330

in vec3 a_Position;
uniform float u_Time;
in vec3 a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV1;

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

float pseudoRandom(float seed) {
    return fract(sin(seed) * 43758.5453123);
}

void falling()
{
	//emitTime
	float newTime = u_Time - pseudoRandom(a_Vel.x);	// -면 태어나지 않은 것
	
	// a_RV 를 0.5 대신 넣으면 됨
	// 원 상에서 지속되는 결과물
	// pseudoRandom(a_RV)

	if(newTime > 0)	// 태어난 것
	{
		float scale = a_RV;
		float t = mod(newTime, 1.0);	// 0~1
		vec4 newPos;
		float initPosX = a_Position.x * scale + sin(a_RV * 2 * c_PI);	// 0~2pi, random initial position
		float initPosY = a_Position.y * scale + cos(a_RV * 2 * c_PI);
		newPos.x = a_Position.x + a_Vel.x * t + 0.5 * c_Gravity.x * t*t;
		newPos.y = a_Position.y + a_Vel.y * t + 0.5 * c_Gravity.y * t*t;
		newPos.z = 0;
		newPos.w = 1;

		gl_Position = newPos;
	}
}

void main()
{
	falling();
}

/* rv 2개 더 추가됐으니
Falling 이라는 API 처럼 하나의 함수에 묶어서 예쁜 이펙트를 만들어줘. */
