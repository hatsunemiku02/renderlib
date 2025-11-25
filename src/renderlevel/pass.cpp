#include "pass.h"
#include "draw.h"
#include "renderlibvulkan/RenderpassVulkan.h"
#include "renderlibvulkan/SwapChainVulkan.h"

Pass::Pass()
	:m_pRenderpassVulkan(nullptr)
	, m_pRenderTarget(nullptr)
	, m_pSwapchainVulkan(nullptr)
{
}

Pass::~Pass()
{
}

void Pass::Compile(const DeviceVulkan& device)
{
	m_pRenderpassVulkan = new RenderpassVulkan();
	if (m_pRenderTarget==nullptr)
	{
		m_pRenderpassVulkan->DefaultCreate(device,*m_pSwapchainVulkan);
	}
}

void Pass::SortDraws()
{
	std::sort(m_pDraws.begin(), m_pDraws.end(),
		[](const Draw* a, const Draw* b) {
			return a->GetSort() < b->GetSort();
		});
}

void Pass::GenPipelineState(const DeviceVulkan& device)
{
	for (int i=0;i< m_pDraws.size();i++)
	{

	}
}

void Pass::Excute()
{

}
