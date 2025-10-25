#pragma once
#include <memory>
class SubMesh;
class Shader;

class RenderObj
{
public:
	RenderObj();
	
	RenderObj(const std::shared_ptr<SubMesh>& pVtx, const std::shared_ptr<Shader>& pShader);
	~RenderObj();

private:
	std::shared_ptr<SubMesh> m_pVertex;
	std::shared_ptr<Shader> m_pShader;
	void* params;

};