#version 330

in vec3 a_Position;
uniform float u_Time;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV1;
in float a_RV2;

out float v_Grey;
out vec3 v_Color;

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
    newPosition.w = 1.0;
	gl_Position = newPosition;
}

float pseudoRandom(float seed) {
    return fract(sin(seed) * 43758.5453123);
}

void falling()
{
	//emitTime
	float newTime = u_Time - a_RV1 * 3;	// -면 태어나지 않은 것
	
	// a_RV 를 0.5 대신 넣으면 됨
	// 원 상에서 지속되는 결과물
	// pseudoRandom(a_RV)

	if(newTime > 0)	// 태어난 것
	{
		float lifeTime = a_RV2;
		float t = mod(newTime, lifeTime);	// 0~1
		float scale = pseudoRandom(a_RV1) * ((lifeTime-t)/lifeTime);
		vec4 newPos;
		float initPosX = a_Position.x * scale + sin(a_RV * 2 * c_PI);	// 0~2pi, random initial position
		float initPosY = a_Position.y * scale + cos(a_RV * 2 * c_PI);
		newPos.x = initPosX + a_Vel.x * t + 0.5 * c_Gravity.x * t*t;
		newPos.y = initPosY + a_Vel.y * t + 0.5 * c_Gravity.y * t*t;
		newPos.z = 0;
		newPos.w = 1;

		gl_Position = newPos;
	}
}

void Thrust()
{
	float newTime = u_Time - a_RV1 * 3;	// -면 태어나지 않은 것
	if(newTime > 0)	// 태어난 것
	{
		float t = mod (newTime, 1.0);	// 0~1
		float ampScale = 0.5 - 0.5 * t;
		float amp = 2 *(a_RV - 0.5) * ampScale;
		float period = a_RV2;
		float sizeScale = 2 - 2 * t;
		vec4 newPosition;

		newPosition.x = a_Position.x * sizeScale
			- 1 + t*2;
		newPosition.y = a_Position.y * sizeScale 
			+ amp*sin(t*2*3.141592 * period);	// 0~2pi
		
		newPosition.z = a_Position.z;
		gl_Position = newPosition;
		v_Grey = 1 - t;	// 시간이 지날수록 어두워짐

		// 진폭을 색상으로 매핑: 진폭 크기가 작을수록 빨강(1,0,0), 클수록 노랑(1,1,0)
		float ampMagnitude = abs(amp); // 0 ~ ampScale
		// ampScale의 최대값은 0.5이므로 0.5로 정규화하면 0..1 범위를 얻음
		float ampNorm = clamp(ampMagnitude / 0.5, 0.0, 1.0);
		// 그린 채널을 ampNorm으로 증가시켜 red->yellow로 변경 (red + green)
		v_Color = vec3(1.0, ampNorm * 2, 0.0);
	}
	else
	{	
		gl_Position = vec4(-100, -100, -100, 1);	// 태어나지 않은 것
		v_Grey = 0;
		v_Color = vec3(0.0);
	}
}

void main()
{
	Thrust();
}

/* rv 2개 더 추가됐으니
Falling 이라는 API 처럼 하나의 함수에 묶어서 예쁜 이펙트를 만들어줘. */
