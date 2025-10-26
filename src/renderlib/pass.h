#pragma once
#include "glad/glad.h"
#include <memory>
#include <vector>
class RenderTarget;
class RenderObj;
class DrawCall;
class Pass
{
public:
	Pass();
	~Pass();

	void SetRenderTarget(const std::shared_ptr<RenderTarget>& p_rt)
	{
		m_pRenderTarget = p_rt;
	}

	void AddRenderObj(const std::shared_ptr<RenderObj>& pRenderObj);

	void SortRenderObjVec2DrawCall();

	void ApplyOpenglAPI();
private:

	std::vector<std::shared_ptr<RenderObj>> m_pRenderObjVector;
	std::vector <std::shared_ptr<DrawCall>> m_pDrawCallVec;
	std::shared_ptr<RenderTarget> m_pRenderTarget;

};