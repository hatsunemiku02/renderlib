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
		static RenderApi s_Instance;
		return s_Instance;
	}

	RenderApi(const RenderApi& api) = delete;
	void operator=(const RenderApi& api) = delete;

	void SetParam(const std::shared_ptr<Param>& pParam);
	void SetShader(const std::shared_ptr<Shader>& pParam);
	void SetMesh(const std::shared_ptr<SubMesh>& pParam);
	void SetSubMesh(const std::shared_ptr<SubMesh>& pParam);
	void Draw();

private:
	unsigned int drawelementstart;
	unsigned int drawelementlength;
	RenderApi()
		:drawelementstart(0)
		, drawelementlength(0)
		, error(0)
	{}

	GLuint error;
};