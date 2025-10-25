#include "submesh.h"

SubMesh::SubMesh()
	:m_Start(0)
	,m_Length(0)
	,m_pVertexBuffer()
{
}

SubMesh::~SubMesh()
{
}

SubMesh::SubMesh(const std::shared_ptr<VertexBuffer>& pVertexBuffer)
	:m_Start(0)
	, m_Length(0)
	, m_pVertexBuffer(pVertexBuffer)
{
}
