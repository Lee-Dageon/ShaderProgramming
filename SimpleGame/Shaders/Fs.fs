#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_Tex;

const float PI = 3.141592;

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

	FragColor = vec4(dist);

	/*if(dist < 0.5)
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
	Circle();
	//FragColor = vec4(sin(v_Tex.x));

}