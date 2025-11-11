#pragma once
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

class DeviceVulkan;

class BufferVulkan
{
public:
	BufferVulkan();
	~BufferVulkan();

	void Allocate(const DeviceVulkan& device,void* data, uint32_t size, VkBufferUsageFlagBits usage);

	void UploadData(const DeviceVulkan& device, void* data, uint32_t size);

	const VkBuffer& const GetBuffer() const
	{
		return m_Buffer;
	}
private:


	VkBuffer m_Buffer;
	VmaAllocation m_Allocation;

};