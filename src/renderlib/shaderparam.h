#pragma once
#include "sortid.h"
#include <string>
#include <glad/glad.h>
const std::string u_World = "r_World";
const std::string u_View = "r_View";
const std::string u_Proj = "r_Proj";


class Param:public SortID
{
public:
	Param():m_ubo(0){}
	~Param() {}

	void CreateUniformBuffer(void* data, uint32_t size);

	GLuint GetGLUbo()
	{
		return m_ubo;
	}
private:

	GLuint m_ubo;


	//uint 
};