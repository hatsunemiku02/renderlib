#pragma once
#include <vector>
#include <glad/glad.h>
#include "sortid.h"


class VertexBuffer:public SortID
{
public:
	VertexBuffer();
	~VertexBuffer();

	void SetData(std::vector<void*>& buffers, std::vector<unsigned int>& buffersize, std::vector<unsigned int>& bufferstride, void* ebobuffer, unsigned int ebolength);

	GLuint GetGLVertexBuffer()
	{
		return m_Vao;
	}
private:
	GLuint m_Vao;
	std::vector<GLuint> m_Vbos;
	GLuint m_Ibo;

};





