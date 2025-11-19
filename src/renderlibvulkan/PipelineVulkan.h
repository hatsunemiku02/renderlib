#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <memory>
class DeviceVulkan;
class RenderpassVulkan;
class ShaderVulkan;
class PipelineLayoutVulkan;
struct VertexInputDescription;



class PipelineVulkan
{
public:

	enum PipelineType
	{
		Graphic,
		Compute
	};

	PipelineVulkan();
	~PipelineVulkan();

	void SetVertexBind(const std::shared_ptr<VertexInputDescription>& bind);

	void SetShaderBind(const ShaderVulkan& vs, const ShaderVulkan& ps);


	void SetPipelineLayout(PipelineLayoutVulkan* pipelineLayout);

	void CreateGraphicPipeline(const DeviceVulkan& device, const RenderpassVulkan& renderpass);

	VkShaderModule createShaderModule(const DeviceVulkan& device, const std::vector<char>& code);


	VkDescriptorSetLayout GetDescSetLayout()
	{
		return m_DescriptorSetLayout;
	}

	VkPipelineLayout GetPipeLineLayout()
	{
		return m_PipelineLayout;
	}

			

	VkPipeline GetPipeline() const
	{
		return m_Pipeline;
	}

	PipelineType GetPipelineType() const
	{
		return m_Type;
	}
private:
	DeviceVulkan* Device;
	VkPipeline m_Pipeline;
	VkPipelineLayout m_PipelineLayout;
	std::shared_ptr<VertexInputDescription>  m_pVertexBind;
	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
	PipelineType m_Type;
	VkDescriptorSetLayout m_DescriptorSetLayout;

	PipelineLayoutVulkan* m_pPipelineLayout;
	//FVulkanLayout* Layout
};


