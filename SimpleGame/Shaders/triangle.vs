#version 330

in vec3 a_Position;

void main()
{
	vec4 newPosition;
	//newPosition = vec4(a_Position.x + 0.5, a_Position.y, a_Position.z, 1);
	newPosition.x = a_Position.x + 0.5;
	newPosition.y = a_Position.y;
	newPosition.z = a_Position.z;

	gl_Position = newPosition;
}
