#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

uniform float u_Time;

const float PI = 3.141592;

const vec2 c_Points[2] = vec2[2](vec2(0.5, 0.5), vec2(0.5, 0.7));

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

	for (int i=0; i<2; i++)
	{
		float newTime = fract(u_Time);	// 0~1
		float oneMinus = 1 - newTime;	// 1~0 °¨¼Ò
		vec2 center = c_Points[i];
		//vec2 center = vec2(0.5, 0.5);
		vec2 currPos = v_Tex;
		float dist = distance(center, currPos);
		float count = 20;
		float range = newTime/3;

		float fade = (1/range) * clamp(range - dist, 0, 1);
		float grey = pow(abs(sin(dist * count * PI - u_Time*5)), 32);
		accum += grey * fade * oneMinus;
		FragColor = vec4(accum);
	}
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


void main()
{
	RainDrop();
	//FragColor = vec4(sin(v_Tex.x));

}