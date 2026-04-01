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
	float period = v_Tex.x * 2 * PI * 5;
	float value = abs(sin(period));
	FragColor = vec4(value);
}

void main()
{
	Line();
	//FragColor = vec4(sin(v_Tex.x));

}