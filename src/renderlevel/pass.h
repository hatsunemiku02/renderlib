#pragma once
#include <vector>

class Draw;
class RederTarget;
class RenderpassVulkan;
class SwapChainVulkan;
class DeviceVulkan;
class Pass
{

public:

	Pass();
	~Pass();

	void SetRenderTarget(RederTarget* pRt)
	{
		m_pRenderTarget = pRt;
	}

	void SetSwapChain(SwapChainVulkan* pRt)
	{
		m_pSwapchainVulkan = pRt;
	}

	void AddDraw(Draw* pDraw)
	{
		m_pDraws.push_back(pDraw);
	}

	void Compile(const DeviceVulkan& device);

	void SortDraws();

	void GenPipelineState(const DeviceVulkan& device);
	
	void Excute();

private:

	RederTarget* m_pRenderTarget;
	std::vector<Draw*> m_pDraws;
	

	RenderpassVulkan* m_pRenderpassVulkan;
	SwapChainVulkan* m_pSwapchainVulkan;

};