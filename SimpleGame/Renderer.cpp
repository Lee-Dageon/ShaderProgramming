#include "stdafx.h"
#include "Renderer.h"
#include <vector>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/triangle.vs", "./Shaders/triangle.fs");
	m_FSShader = CompileShaders("./Shaders/FS.vs", "./Shaders/FS.fs");
	
	GenParticles(5000);

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float centerX = 0;
	float centerY = 0;
	float size = 0.1;
	float mass = 1;

	// 속도
	float vx = 1;
	float vy = 1;

	float triangle[]
		=
	{
		centerX - size / 2, centerY - size / 2, 0,	//v0
		mass, vx, vy,
		centerX + size / 2, centerY - size / 2, 0,
		mass, vx, vy,//v1
		centerX + size / 2, centerY + size / 2, 0,
		mass,vx, vy,//v2 triangle 1

		centerX - size / 2, centerY - size / 2, 0,
		mass,vx, vy,//v0
		centerX + size / 2, centerY + size / 2, 0,
		mass,vx, vy,	//v1
		centerX - size / 2, centerY + size / 2, 0,
		mass, vx, vy	//v2 triangle 2
	};

	glGenBuffers(1, &m_TriangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);

	//GPU 메모리에 올림
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	float rectFS[] //x,y,z, tx, ty:stride 5
		=
	{
		-1, -1, 0, 0, 1,
		1, 1, 0, 1, 0,
		-1, 1, 0, 0, 0, // Triangle1

		-1, -1, 0, 0, 1,
		1, -1, 0, 1, 1,
		1, 1, 0, 1, 0, // Triangle2
	};

	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);

	//GPU 메모리에 올림
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectFS), rectFS, GL_STATIC_DRAW);

}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float gTime = 0;

void Renderer::DrawTriangle()
{
	gTime += 0.0001f;
	//Program select
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime);

	//stride 설정
	int attribPosition = glGetAttribLocation(m_TriangleShader,
		"a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader,
		"a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader,
		"a_Vel");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glVertexAttribPointer(
		attribPosition, 3, /*세 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		6 * sizeof(float), /*start position*/ 0);
	glVertexAttribPointer(
		attribMass, 1, /*하나씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		6 * sizeof(float), (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(
		attribVel, 2, /*두 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		6 * sizeof(float), (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::DrawParticles()
{
	gTime += 0.0001f; // 시간 증가 (애니메이션 효과를 위해)
	// Program select - 파티클 렌더링에 m_TriangleShader를 재사용합니다.
	glUseProgram(m_TriangleShader);

	// u_Time uniform은 이미 설정되었을 수 있으나, 파티클에도 필요하다면 다시 설정
	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime); // gTime은 Renderer.cpp 전역 변수

	// attribute 위치 가져오기 (셰이더에 정의된 이름과 매칭)
	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	int attribRV = glGetAttribLocation(m_TriangleShader, "a_RV");
	int attribRV1 = glGetAttribLocation(m_TriangleShader, "a_RV1");
	int attribRV2 = glGetAttribLocation(m_TriangleShader, "a_RV2");

	// attribute 배열 활성화
	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);
	glEnableVertexAttribArray(attribRV);
	glEnableVertexAttribArray(attribRV1);
	glEnableVertexAttribArray(attribRV2);

	// 파티클 VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);

	// 파티클 위치 attribute 설정 (stride 및 offset은 GenParticles의 Vertex struct와 일치)
	glVertexAttribPointer(
		attribPosition, 3, /*세 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), /*start position*/ 0
	);

	// 파티클 질량 attribute 설정
	glVertexAttribPointer(
		attribMass, 1, /*하나씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (GLvoid*)(sizeof(float) * 3)
	);

	// 파티클 속도 attribute 설정
	glVertexAttribPointer(
		attribVel, 2, /*두 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (GLvoid*)(sizeof(float) * 4)
	);

	// 파티클 RV attribute 설정
	glVertexAttribPointer(
		attribRV, 1, /*한 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (GLvoid*)(sizeof(float) * 6)
	);

	// 파티클 RV1 attribute 설정
	glVertexAttribPointer(
		attribRV1, 1, /*한 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (GLvoid*)(sizeof(float) * 7)
	);

	// 파티클 RV2 attribute 설정
	glVertexAttribPointer(
		attribRV2, 1, /*한 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (GLvoid*)(sizeof(float) * 8)
	);

	// 파티클 그리기
	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticleCount); // GenParticles에서 계산한 총 정점 개수만큼 그림

	// attribute 배열 비활성화
	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRV);
	glDisableVertexAttribArray(attribRV1);
	glDisableVertexAttribArray(attribRV2);

	// 현재 바인딩된 프레임버퍼를 기본 프레임버퍼로 되돌림 (DrawTriangle에 따라 동일하게)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GenParticles(int num)
{
	// particle 데이터
	struct Vertex
	{
		float x, y, z;
		float mass;
		float vx, vy;
		float rv;
		float rv1;
		float rv2;
	};

	// num * 6개의 정점 데이터를 담을 벡터(동적 배열)
	std::vector<Vertex> vertices;
	vertices.resize(num * 6); // 파티클 하나당 6개의 정점이므로 총 크기를 미리 할당

	for (int i = 0; i < num; i++)
	{
		// 각 파티클의 초기 위치, 크기, 질량, 속도를 랜덤하게 생성
		float centerX = 0.0f;
		float centerY = 0.0f;
		float size = 0.01;
		float mass = 1;

		// x축 속도를 0으로 설정하여 양옆으로 튀지 않게 합니다.
		float vx = 0.0f;
		// 위로 튀고 떨어지는 느낌을 줄이려면 y축 초기 속도(vy)도 0.0f 또는 음수로 조절할 수 있습니다.
		float vy = 0.0f; 

		float rv = ((rand() % 100)) / 100.0f; // 0 ~ 1 사이의 랜덤 값
		float rv1 = ((rand() % 100)) / 100.0f;
		float rv2 = ((rand() % 100)) / 100.0f;

		// 파티클 하나(사각형)를 구성하는 6개의 정점 데이터 생성
		Vertex v[6];
		// Triangle 1
		v[0] = { centerX - size / 2, centerY - size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v0
		v[1] = { centerX + size / 2, centerY - size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v1
		v[2] = { centerX + size / 2, centerY + size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v2
		// Triangle 2
		v[3] = { centerX - size / 2, centerY - size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v0
		v[4] = { centerX + size / 2, centerY + size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v2
		v[5] = { centerX - size / 2, centerY + size / 2, 0.0f, mass, vx, vy, rv, rv1, rv2 }; // v3

		// 생성된 6개의 정점 데이터를 전체 벡터에 복사
		memcpy(&vertices[i * 6], v, sizeof(Vertex) * 6);
	}

	// 이전에 생성된 VBO가 있다면 삭제
	if (m_ParticleVBO != 0)
	{
		glDeleteBuffers(1, &m_ParticleVBO);
	}

	// VBO 생성 및 데이터 업로드
	glGenBuffers(1, &m_ParticleVBO); // VBO 핸들(ID) 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO); // 생성한 VBO를 현재 작업 대상으로 지정

	// CPU에 있는 전체 정점 데이터를 GPU의 VBO로 복사
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num * 6, vertices.data(), GL_STATIC_DRAW);

	// 총 정점 개수를 멤버 변수에 저장
	m_VBOParticleCount = num * 6;
}

void Renderer::DrawFS()
{
	gTime += 0.0001f;
	//Program select
	glUseProgram(m_FSShader);

	int uTime = glGetUniformLocation(m_FSShader, "u_Time");
	glUniform1f(uTime, gTime);

	//stride 설정
	int attribPosition = glGetAttribLocation(m_FSShader,
		"a_Pos");
	//stride 설정
	int attribTex = glGetAttribLocation(m_FSShader,
		"a_Tex");

	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glVertexAttribPointer(
		attribPosition, 3, /*세 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		5 * sizeof(float), /*start position*/ 0);

	glVertexAttribPointer(
		attribTex, 2, /*두 개씩 읽어라*/
		GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (GLvoid*)sizeof(float));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}