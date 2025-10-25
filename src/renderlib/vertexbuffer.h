#pragma once
#include <vector>
#include <glad/glad.h>



class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void SetData(std::vector<void*>& buffers, std::vector<unsigned int>& buffersize, std::vector<unsigned int>& bufferstride, void* ebobuffer, unsigned int ebolength);
private:
	GLuint m_Vao;
	std::vector<GLuint> m_Vbos;
	GLuint m_Ibo;

};





