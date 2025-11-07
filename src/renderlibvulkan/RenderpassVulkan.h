#pragma once
#include "vulkan/vulkan.h"
#include <vector>

class SwapChainVulkan;
class DeviceVulkan;


class RenderpassVulkan
{
	
public:
	RenderpassVulkan();
	~RenderpassVulkan();


	void DefaultCreate(const DeviceVulkan& device ,const SwapChainVulkan& swapChain);

	VkRenderPass GetRenderpass() const
	{
		return renderPass;
	}
private:
	VkRenderPass renderPass;
};