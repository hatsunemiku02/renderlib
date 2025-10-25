#include "vertexbuffer.h"

VertexBuffer::VertexBuffer()
	: m_Vao(0)
	, m_Vbos()
	, m_Ibo(0)
{
	glGenVertexArrays(1, &m_Vao);
	//glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_Ibo);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteVertexArrays(1, &m_Vao);
	glDeleteBuffers(m_Vbos.size(), m_Vbos.data());
	glDeleteBuffers(1, &m_Ibo);

}

void VertexBuffer::SetData(std::vector<void*>& buffers,std::vector<unsigned int>& buffersize,std::vector<unsigned int>& bufferstride,void* ebobuffer,unsigned int ebolength )
{
	unsigned int vbosize = buffers.size();
	glBindVertexArray(m_Vao);

	std::vector<GLintptr> offsets;
	
	for (int i=0;i< vbosize;i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[i]);
		glBufferData(GL_ARRAY_BUFFER, buffersize[i], buffers[i], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		offsets[i] = 0;
	}
	
	glBindVertexBuffers(0, vbosize, m_Vbos.data(), offsets.data(), (GLsizei*)bufferstride.data());
	
	for (int i = 0; i < vbosize; i++)
	{
		glVertexAttribFormat(i, bufferstride[i], GL_FLOAT, GL_FALSE, 0); // Î»ÖÃ
		glVertexAttribBinding(i, 0);
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebolength, ebobuffer, GL_STATIC_DRAW);


	glBindVertexArray(0);
}