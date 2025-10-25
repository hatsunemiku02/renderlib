#pragma once
#include <memory>
class SubMesh;
class Shader;
class Param;

class RenderObj
{
public:
	RenderObj();
	
	RenderObj(const std::shared_ptr<SubMesh>& pVtx, const std::shared_ptr<Shader>& pShader, const std::shared_ptr<Param>& pParam);
	~RenderObj();

	const std::shared_ptr<SubMesh>& GetSubMesh()
	{
		return m_pSubMesh;
	}

	const std::shared_ptr<Shader>& GetShader()
	{
		return m_pShader;
	}

	const std::shared_ptr<Param>& GetParam()
	{
		return m_pParam;
	}

private:
	std::shared_ptr<SubMesh> m_pSubMesh;
	std::shared_ptr<Shader> m_pShader;
	std::shared_ptr<Param> m_pParam;

};