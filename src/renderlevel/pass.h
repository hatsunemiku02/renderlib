#pragma once
#include <vector>

class Draw;
class RederTarget;

class Pass
{

public:

	Pass();
	~Pass();

	void SetRenderTarget(RederTarget* pRt)
	{
		m_pRenderTarget = pRt;
	}

	void AddDraw(Draw* pDraw)
	{
		m_pDraws.push_back(pDraw);
	}

private:

	RederTarget* m_pRenderTarget;
	std::vector<Draw*> m_pDraws;

};