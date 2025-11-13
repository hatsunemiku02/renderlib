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

void BufferVulkan::Allocate(const DeviceVulkan& device, void* data,uint32_t size, VmaMemoryUsage memUsage, uint32_t usageflagbits)
{
	Allocate(device,   size,  memUsage,  usageflagbits);

	//add the destruction of triangle mesh buffer to the deletion queue
	//_mainDeletionQueue.push_function([=]() {
	//
	//	vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
	//	});

	UploadData(device, data, size);

}

void BufferVulkan::Allocate(const DeviceVulkan& device, uint32_t size, VmaMemoryUsage memUsage, uint32_t usageflagbits)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//this is the total size, in bytes, of the buffer we are allocating
	bufferInfo.size = size;
	//this buffer is going to be used as a Vertex Buffer
	bufferInfo.usage = usageflagbits;


	//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = memUsage;

	//allocate the buffer
	vmaCreateBuffer(device.GetAllocator(), &bufferInfo, &vmaallocInfo,
		&m_Buffer,
		&m_Allocation,
		nullptr);
}

void BufferVulkan::UploadData(const DeviceVulkan& device, void* data, uint32_t size)
{
	void* datagpu;
	vmaMapMemory(device.GetAllocator(), m_Allocation, &datagpu);

	memcpy(datagpu, data, size);

	vmaUnmapMemory(device.GetAllocator(), m_Allocation);
}

void BufferVulkan::Destory(const DeviceVulkan& device)
{
	vmaDestroyBuffer(device.GetAllocator(), m_Buffer, m_Allocation);
}
