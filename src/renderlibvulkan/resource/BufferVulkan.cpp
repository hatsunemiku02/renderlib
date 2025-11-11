#include "BufferVulkan.h"
#include "../DeviceVulkan.h"

BufferVulkan::BufferVulkan()
	:m_Buffer()
	, m_Allocation()
{
}

BufferVulkan::~BufferVulkan()
{
}

void BufferVulkan::Allocate(const DeviceVulkan& device, void* data,uint32_t size, VkBufferUsageFlagBits usage)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//this is the total size, in bytes, of the buffer we are allocating
	bufferInfo.size = size;
	//this buffer is going to be used as a Vertex Buffer
	bufferInfo.usage = usage;


	//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	//allocate the buffer
	vmaCreateBuffer(device.GetAllocator(), &bufferInfo, &vmaallocInfo,
		&m_Buffer,
		&m_Allocation,
		nullptr);

	//add the destruction of triangle mesh buffer to the deletion queue
	//_mainDeletionQueue.push_function([=]() {
	//
	//	vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
	//	});

	UploadData(device, data, size);

}

void BufferVulkan::UploadData(const DeviceVulkan& device, void* data, uint32_t size)
{
	void* datagpu;
	vmaMapMemory(device.GetAllocator(), m_Allocation, &datagpu);

	memcpy(datagpu, data, size);

	vmaUnmapMemory(device.GetAllocator(), m_Allocation);
}
