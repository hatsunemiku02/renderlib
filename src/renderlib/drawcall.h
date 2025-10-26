#pragma once
#include <memory>
#include <vector>

class Shader;
class SubMesh;
class RenderObj;
class Param;

class DrawCall
{
public:
	DrawCall();
	~DrawCall();

	std::shared_ptr<Shader> GetShader();

	std::shared_ptr<Param> GetParam();

	std::shared_ptr<SubMesh> GetSubMesh();

	unsigned int GetInstanceCount();

	void AddRenderObj(const std::shared_ptr<RenderObj>& pRenderObj);

	void SetSameShader(bool same)
	{
		m_bSameShader = same;
	}

	void SetSameParam(bool same)
	{
		m_bSameParam = same;
	}

	void SetSameSubMesh(bool same)
	{
		m_bSameSubMesh = same;
	}

	bool GetSameShader()
	{
		return m_bSameShader;
	}

	bool GetSameParam()
	{
		return m_bSameParam;
	}

	bool GetSameSubMesh()
	{
		return m_bSameSubMesh;
	}

private:
	std::vector<std::shared_ptr<RenderObj>> m_RenderObjVector;
	bool m_bSameShader;
	bool m_bSameParam;
	bool m_bSameSubMesh;
};