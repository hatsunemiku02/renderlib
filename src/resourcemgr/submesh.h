#pragma once
#include <memory>
#include "renderlib/sortid.h"

class VertexBuffer;

class SubMesh:public SortID
{
public:
	SubMesh();
	SubMesh(const std::shared_ptr<VertexBuffer>& pVertexBuffer);
	~SubMesh();

	void BuildDefaultBox();

	std::shared_ptr<VertexBuffer>& GetVertexBuffer()
	{
		return m_pVertexBuffer;
	}

	void SetStart(unsigned int start)
	{
		m_Start = start;
	}

	void SetLength(unsigned int length)
	{
		m_Length = length;
	}

	unsigned int GetStart()
	{
		return m_Start;
	}

	unsigned int GetLength()
	{
		return m_Length;
	}
private:
	unsigned int m_Start;
	unsigned int m_Length;
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;
};