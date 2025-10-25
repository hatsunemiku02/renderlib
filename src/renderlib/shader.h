#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader() {};
	~Shader() {};

	void BuildDefaultShader();

	void BuildShader(const std::string& vs,const std::string& fs);

private:
	GLuint m_Program;

};



