#pragma once
#include "glad/glad.h"
#include <memory>
class RenderTarget;

class Pass
{
public:
	Pass();
	~Pass();

	void SetRenderTarget(const std::shared_ptr<RenderTarget>* p_rt)
	{
		m_pRenderTarget = p_rt;
	}

	void AddRenderObj()

private:

	std::shared_ptr<RenderTarget> m_pRenderTarget;

};