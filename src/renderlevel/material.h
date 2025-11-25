#pragma once
#include <vector>
#include <unordered_map>
#include "handle.h"
class Param;
class Shader;


class Material:public Handle
{
public:

	enum ShaderStage
	{
		vertex,
		fragment
	};
	Material();
	~Material();

	void AddParam(Param* pParam)
	{
		m_pParams.push_back(pParam);
	}
	void AddShader(ShaderStage stage, Shader* pShader)
	{
		m_ShaderMap[stage] = pShader;
	}

private:
	std::vector<Param*> m_pParams;
	std::unordered_map<ShaderStage, Shader*> m_ShaderMap;

};