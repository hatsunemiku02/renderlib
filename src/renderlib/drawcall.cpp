#include "drawcall.h"
#include "renderobj.h"

DrawCall::DrawCall()
	:m_bSameShader(false)
	,m_bSameParam(false)
	,m_bSameSubMesh(false)
	,m_RenderObjVector()
{
}

DrawCall::~DrawCall()
{
}



void DrawCall::AddRenderObj(const std::shared_ptr<RenderObj>& pRenderObj)
{
	m_RenderObjVector.push_back(pRenderObj);
}

std::shared_ptr<Shader> DrawCall::GetShader()
{
	return m_RenderObjVector[0]->GetShader();
}

std::shared_ptr<Param> DrawCall::GetParam()
{
	return m_RenderObjVector[0]->GetParam();
}

std::shared_ptr<SubMesh> DrawCall::GetSubMesh()
{
	return m_RenderObjVector[0]->GetSubMesh();
}

unsigned int DrawCall::GetInstanceCount()
{
	return m_RenderObjVector.size();
}