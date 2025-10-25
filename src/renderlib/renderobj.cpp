#include "renderobj.h"

RenderObj::RenderObj()
 :m_pVertex()
 ,m_pShader()
 ,params(nullptr)
{
}

RenderObj::RenderObj(const std::shared_ptr<SubMesh>& pVtx, const std::shared_ptr<Shader>& pShader)
	:m_pVertex(pVtx)
	, m_pShader(pShader)
	, params(nullptr)
{
}

RenderObj::~RenderObj()
{
}
