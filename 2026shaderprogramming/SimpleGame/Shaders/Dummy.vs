#version 330

uniform float u_Time;
uniform vec4 u_DropInfo[256];  

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

void Circles()
{	
	float accum = 0;
	for(int i = 0; i< 256; i++)
	{
		vec2 center = u_DropInfo[i].xy - vec2(0.5,0.5);
		vec2 pos = a_Pos.xy;
		float lTime = u_DropInfo[i].z;
		float sTime = u_DropInfo[i].w;
		float nTime = u_Time - sTime;

		if(nTime >0)
		{
			float nVal = fract(nTime/lTime); //0~1
			float oneMinus = 1.0 - nVal; //1~0
			float t = nVal*lTime; // 0~lTime
			float d = distance(center, pos);

			float range = t/5.0;

			float fade = 30 * clamp(range - d, 0, 1.0);

			float sinValue = pow(abs(sin(d*4*c_PI*8 + t * 2)),3.0);
			accum += sinValue * fade* oneMinus;
		}
	}

	v_Grey = accum;
	gl_Position = vec4(a_Pos, 1.0);

}

void main()
{
	Circles();
}
