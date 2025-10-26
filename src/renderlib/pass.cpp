#include "pass.h"
#include "renderobj.h"
#include "shader.h"
#include "shaderparam.h"
#include "resourcemgr/submesh.h"
#include "drawcall.h"
#include "renderlib/renderapi.h"
#include<algorithm>
Pass::Pass()
{
}

Pass::~Pass()
{
}

void Pass::AddRenderObj(const std::shared_ptr<RenderObj>& pRenderObj)
{
	m_pRenderObjVector.push_back(pRenderObj);
}
/*
≈≈–ÚπÊ‘Ú
1.shader
2.submesh
3.param
*/
void Pass::SortRenderObjVec2DrawCall()
{
	std::sort(m_pRenderObjVector.begin(), m_pRenderObjVector.end(), 
		[](const std::shared_ptr<RenderObj>& objA, const std::shared_ptr<RenderObj>& objB)->bool
		{
			if (objA->GetShader()->GetSortID()!= objB->GetShader()->GetSortID())
			{
				return objA->GetShader()->GetSortID() < objB->GetShader()->GetSortID();
			}
			if (objA->GetSubMesh()->GetSortID() != objB->GetSubMesh()->GetSortID())
			{
				return objA->GetSubMesh()->GetSortID() < objB->GetSubMesh()->GetSortID();
			}
			return objA->GetParam()->GetSortID() < objB->GetParam()->GetSortID();
		});


	std::shared_ptr<Shader> pCurShader = m_pRenderObjVector[0]->GetShader();
	std::shared_ptr<SubMesh> pCurSubMesh = m_pRenderObjVector[0]->GetSubMesh();
	std::shared_ptr<Param> pCurParam = m_pRenderObjVector[0]->GetParam();

	m_pDrawCallVec.emplace_back(std::make_shared<DrawCall>());
	m_pDrawCallVec[0]->AddRenderObj(m_pRenderObjVector[0]);
	
	
	for (int i=1;i< m_pRenderObjVector.size();i++)
	{
		std::shared_ptr<DrawCall> lastDrawCall = m_pDrawCallVec[m_pDrawCallVec.size() - 1];
		std::shared_ptr<RenderObj> renderObjPtr = m_pRenderObjVector[i];

		bool sameShader = renderObjPtr->GetShader() == lastDrawCall->GetShader();
		bool sameSubMesh = renderObjPtr->GetSubMesh() == lastDrawCall->GetSubMesh();
		bool sameParam = renderObjPtr->GetParam() == lastDrawCall->GetParam();
		
		if(sameShader
		&& sameSubMesh
		&& sameParam)
		{
			lastDrawCall->AddRenderObj(renderObjPtr);
		}
		else
		{
			m_pDrawCallVec.emplace_back(std::make_shared<DrawCall>());
			lastDrawCall = m_pDrawCallVec[m_pDrawCallVec.size() - 1];
			lastDrawCall->AddRenderObj(renderObjPtr);
			lastDrawCall->SetSameShader(sameShader);
			lastDrawCall->SetSameSubMesh(sameSubMesh);
			lastDrawCall->SetSameParam(sameParam);
		}
	}
}

void Pass::ApplyOpenglAPI()
{
	RenderApi& renderApi = RenderApi::GetInstance();
	for (int i=0;i< m_pDrawCallVec.size();i++)
	{
		if (!m_pDrawCallVec[i]->GetSameShader())
		{
			renderApi.SetShader(m_pDrawCallVec[i]->GetShader());
		}
		if (!m_pDrawCallVec[i]->GetSameParam())
		{
			renderApi.SetParam(m_pDrawCallVec[i]->GetParam());
		}
		if (!m_pDrawCallVec[i]->GetSameSubMesh())
		{
			renderApi.SetSubMesh(m_pDrawCallVec[i]->GetSubMesh());
		}

		for (int j=0;j< m_pDrawCallVec[i]->GetInstanceCount();j++)
		{
			renderApi.Draw();
		}
	}
}
