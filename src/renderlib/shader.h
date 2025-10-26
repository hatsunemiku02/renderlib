#pragma once
#include <glad/glad.h>
#include <string>
#include "sortid.h"

class Shader:public SortID
{
public:
	Shader():m_Program(0){};
	~Shader() {};

	void BuildDefaultShader();

	void BuildShader(const std::string& vs,const std::string& fs);

	GLuint GetGLShader()
	{
		return m_Program;
	}
private:
	GLuint m_Program;

};



