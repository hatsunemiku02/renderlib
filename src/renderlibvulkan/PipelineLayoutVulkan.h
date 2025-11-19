#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <memory>


class DeviceVulkan;
class PipelineLayoutVulkan
{
public:
	PipelineLayoutVulkan();
	~PipelineLayoutVulkan();

	void CreatePipelineLayout(const DeviceVulkan& device);

	VkPipelineLayout GetPipelineLayout() const
	{
		return m_PipelineLayout;
	}

	VkDescriptorSetLayout GetDescSetLayout() const
	{
		return m_DescriptorSetLayout;
	}
private:

	VkPipelineLayout m_PipelineLayout;
	VkDescriptorSetLayout m_DescriptorSetLayout;

};