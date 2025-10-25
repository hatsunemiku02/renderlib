#include "renderobj.h"
#include "resourcemgr/submesh.h"
#include "shader.h"
#include "shaderparam.h"

RenderObj::RenderObj()
 :m_pSubMesh()
 ,m_pShader()
 , m_pParam(nullptr)
{
}

RenderObj::RenderObj(const std::shared_ptr<SubMesh>& pVtx, const std::shared_ptr<Shader>& pShader,const std::shared_ptr<Param>& pParam)
	:m_pSubMesh(pVtx)
	, m_pShader(pShader)
	, m_pParam(pParam)
{

}

RenderObj::~RenderObj()
{
}
