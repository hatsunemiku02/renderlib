#pragma once
#include "glad/glad.h"
#include <memory>


class Param;
class Shader;
class SubMesh;

class RenderApi
{
public:
	static RenderApi& GetInstance()
	{
		return s_Instance;
	}

	RenderApi(const RenderApi& api) = delete;
	void operator=(const RenderApi& api) = delete;

	void SetParam(const std::shared_ptr<Param>& pParam);
	void SetShader(const std::shared_ptr<Shader>& pParam);
	void DrawSubMesh(const std::shared_ptr<SubMesh>& pParam);
	void Draw();

private:
	unsigned int drawelementstart;
	unsigned int drawelementlength;
	static RenderApi s_Instance;
	RenderApi(){}
};