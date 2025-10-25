#pragma once
#include <memory>

class VertexBuffer;

class SubMesh
{
public:
	SubMesh();
	~SubMesh();
	SubMesh(const std::shared_ptr<VertexBuffer>& pVertexBuffer);



private:
	int m_Start;
	int m_Length;
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;
};