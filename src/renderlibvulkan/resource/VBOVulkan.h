#pragma once

#include "BufferVulkan.h"
#include <vector>
#include <memory>

class DeviceVulkan;

struct VertexInputDescription {

	std::vector<VkVertexInputBindingDescription> bindings;
	std::vector<VkVertexInputAttributeDescription> attributes;

	VkPipelineVertexInputStateCreateFlags flags = 0;
};

class VBOVulkan
{
public:
	VBOVulkan();
	~VBOVulkan();


	void DefaultCreate(const DeviceVulkan& device);
	void CreateVBO(const DeviceVulkan& device,void* data,uint32_t size);
	void SetVBODesc(const std::shared_ptr<VertexInputDescription>& desc);

	const std::shared_ptr<VertexInputDescription>& GetVertexDesc() const
	{
		return m_pVertexDesc;
	}
private:
	BufferVulkan m_VertexBuffer;
	std::shared_ptr<VertexInputDescription>    m_pVertexDesc;

};