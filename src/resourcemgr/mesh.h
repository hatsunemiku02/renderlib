#pragma once
#include <memory>
#include "renderlib/vertexbuffer.h"


class mesh
{
public:
	mesh() {};
	~mesh() {};

	void BuildDefaultBox();



private:
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;

};