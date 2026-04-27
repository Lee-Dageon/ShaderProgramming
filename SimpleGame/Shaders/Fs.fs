#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

uniform float u_Time; // 현재 시간 (초 단위)
uniform vec4 u_Points[500];
uniform sampler2D u_Texture;

const float PI = 3.141592;

const vec4 c_Points[2] = vec4[2](vec4(0.5, 0.5, 0, 0.5), vec4(0.5, 0.7, 0.5, 1));

void Simple()
{
	if(v_Tex.x<0.5)
	{
		FragColor = vec4(0);
	}
	else
	{ 		
		FragColor = vec4(1);
	}

}

void Line()
{
	//FragColor = vec4(v_Tex.xy, 0, 1);
	float trans = PI/2;
	float periodX = (v_Tex.x * 2 * PI - trans) * 5;
	float periodY = (v_Tex.y * 2 * PI - trans) * 5;
	float valueX = pow(abs(sin(periodX + periodY)), 16);
	float valueY = pow(abs(sin(periodY-periodX+1)), 16);
	FragColor = vec4(max(valueX, valueY));
}

void Circle()
{
	vec2 center = vec2(0.5, 0.5);
	float dist = distance(center, v_Tex);
	float width =0.01;
	float radius = 0.5;

	if(dist > (radius - width) && dist < radius)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}
}
  
void RainDrop()
{
	float accum = 0;

	for (int i=0; i<500; i++)
	{
		float startTime = u_Points[i].z;
		float lifeTime = u_Points[i].w;
		float newTime = u_Time - startTime;
		if(newTime > 0)
		{
			float t = fract(newTime/lifeTime); // 0~1 �ݺ�
			float oneMinus = 1 - t;	// 1~0 ����
			t = t * lifeTime;	// 0~lifeTime
			vec2 center = u_Points[i].xy;
			//vec2 center = vec2(0.5, 0.5);
			vec2 currPos = v_Tex;
			float dist = distance(center, currPos);
			float count = 20;
			float range = t/4;

			float fade = (1/range) * clamp(range - dist, 0, 1);
			float grey = pow(abs(sin(dist * count * PI - t*100)), 32);
			accum += grey * fade * oneMinus;
		}
	}	
	FragColor = vec4(accum);
}


void Circles()
{
	vec2 center = vec2(0.5, 0.5);
	float dist = distance(center, v_Tex);
	float width = 0.01;
	float radius = 0.5;
	float count = 10;
	float gray = pow(abs(sin(dist * count * PI - u_Time*10)), 32);


	FragColor = vec4(gray);


	/*if(dist > (radius - width) && dist < radius)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}*/
}

void Flame()
{
	float amp = 0.5;
	float speed = 30;
	float newY = 1.0 - v_Tex.y;
	// 방법 1: u_Points 배열 활용 (주석 처리)
	//float rv = u_Points[0].x; 
	//float sinInput = newY * PI * 2 - (u_Time * speed) + (rv * 10.0);
	
	// 방법 2: 셰이더 내부 노이즈 생성 (현재 적용)
	float noise = fract(sin(dot(vec2(newY, u_Time * 0.1), vec2(12.9898, 78.233))) * 43758.5453);
	float sinInput = newY * PI * 2 - (u_Time * speed) + (noise);
	float sinValue = newY * amp*(((sin(sinInput) + 1) / 2) - 0.5)+0.5;
	
	float grey = 0;

	float fWidth = 0.0;
	float width = 0.5 * mix(fWidth, 1, newY);

	// 중심축(sinValue)으로부터의 거리 계산
	float distFromCenter = abs(v_Tex.x - sinValue);

	// newY가 0에 가까워질수록 갈라지는 틈의 크기를 결정
	float split = newY * width * 0.1; 

	if(v_Tex.x < sinValue + width/2 && v_Tex.x > sinValue - width/2)
	
	// 중심 거리(distFromCenter)가 split보다 크고 전체 너비의 절반보다 작을 때만 표시
	if(distFromCenter < width/2 && distFromCenter > split)
	{
		grey = 1;
	} 
	else
	{
		grey = 0;
		discard;
	}
	FragColor = vec4(grey);
}

void Flag()
{
	float amp = 0.5;
	float speed = 30;
	float sinInput = v_Tex.x * PI * 2 - (u_Time * speed);
	float sinValue = v_Tex.x * amp*(((sin(sinInput) + 1) / 2) - 0.5)+0.5;
	
	float grey = 0;

	float fWidth = 0.0;
	float width = 0.5 * mix(1, fWidth, v_Tex.x);


	if(v_Tex.y < sinValue + width/2 && v_Tex.y > sinValue - width/2)
	{
		grey = 1;
	} 
	else
	{
		grey = 0;
		discard;
	}
	FragColor = vec4(grey);
}

void TextureQ1()
{
	float tx = v_Tex.x;
	float ty = 1-abs((v_Tex.y * 2) -1);

	vec2 tex = vec2(tx, ty);
	FragColor =	texture(u_Texture, tex);
}

void TextureQ2()
{
	// x: 0~1, 0~1, 0~1 
	// fract(0~1) * 3

	float tx = fract(v_Tex.x*3);
	float ty = v_Tex.y / 3;
	
	float offsetX = 0;
	float offsetY = (2-floor(v_Tex.x * 3))/3;

	vec2 tex = vec2(offsetX + tx, offsetY + ty);
	FragColor =	texture(u_Texture, tex);
}

void TextureQ3()
{
	// x: 0~1, 0~1, 0~1 
	// fract(0~1) * 3

	float tx = fract(v_Tex.x*3);
	float ty = v_Tex.y / 3;
	
	float offsetX = 0;
	float offsetY = floor(v_Tex.x * 3)/3;

	vec2 tex = vec2(offsetX + tx, offsetY + ty);
	FragColor =	texture(u_Texture, tex);
}

void main()
{
	TextureQ3();
}