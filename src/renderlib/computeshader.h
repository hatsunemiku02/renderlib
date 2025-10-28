#pragma once
#include "glad/glad.h"
#include "sortid.h"
#include <string>

class ComputeShader :public SortID
{


public:
	ComputeShader() :m_Program(0) {};
	~ComputeShader() {};

	void BuildDefaultShader();

	void BuildShader(const std::string& cs);

	GLuint GetGLShader()
	{
		return m_Program;
	}

private:
	GLuint m_Program;


};