#pragma once
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

class DeviceVulkan;

class BufferVulkan
{
public:
	BufferVulkan();
	~BufferVulkan();

	void Allocate(const DeviceVulkan& device,void* data, uint32_t size, VmaMemoryUsage memUsage, uint32_t usageflagbits);

	void Allocate(const DeviceVulkan& device, uint32_t size, VmaMemoryUsage memUsage, uint32_t usageflagbits);

	void UploadData(const DeviceVulkan& device, void* data, uint32_t size);

	void Destory(const DeviceVulkan& device);

	const VkBuffer& const GetBuffer() const
	{
		return m_Buffer;
	}
private:


	VkBuffer m_Buffer;
	VmaAllocation m_Allocation;



	//VkFence m_UploadFence;
	//VkCommandPool m_CommandPool;


};