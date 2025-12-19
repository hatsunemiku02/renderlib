#include "shaderparam.h"



void Param::CreateUniformBuffer(void* data, uint32_t size)
{
	
	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

