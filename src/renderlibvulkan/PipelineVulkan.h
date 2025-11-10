#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <memory>
class DeviceVulkan;
class RenderpassVulkan;
struct VertexInputDescription;

class PipelineVulkan
{
public:
	PipelineVulkan();
	~PipelineVulkan();

	void SetVertexBind(const std::shared_ptr<VertexInputDescription>& bind);

	void CreateGraphicPipeline(const DeviceVulkan& device, const RenderpassVulkan& renderpass, const std::vector<char>& vs, const std::vector<char>& ps);

	VkShaderModule createShaderModule(const DeviceVulkan& device, const std::vector<char>& code);

	VkPipeline GetPipeline()
	{
		return m_Pipeline;
	}
private:
	DeviceVulkan* Device;
	VkPipeline m_Pipeline;
	VkPipelineLayout m_PipelineLayout;
	std::shared_ptr<VertexInputDescription>  m_pVertexBind;
	//FVulkanLayout* Layout
};


