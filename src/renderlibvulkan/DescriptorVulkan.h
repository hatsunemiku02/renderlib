#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <memory>


class DeviceVulkan;
class PipelineLayoutVulkan;
class TextureVulkan;

class DescriptorVulkan
{
public:
	DescriptorVulkan();
	~DescriptorVulkan();

	void CreateDescriptor(const DeviceVulkan& device,const PipelineLayoutVulkan& layout);


	void UpdateTexture(const DeviceVulkan& device, const TextureVulkan& texture);

	uint32_t GetDescSetCount() const 
	{
		return m_DescriptorSets.size();
	}

	const VkDescriptorSet* GetDescSetPtr() const
	{
		return m_DescriptorSets.data();
	}
private:

	std::vector<VkDescriptorSet> m_DescriptorSets;
	VkDescriptorPool m_DescriptorPool;
};