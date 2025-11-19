#include "GpuOnlyResVulkan.h"
#include "BufferTransfer.h"
#include "../DeviceVulkan.h"

GpuOnlyResVulkan::GpuOnlyResVulkan()
{
}

GpuOnlyResVulkan::~GpuOnlyResVulkan()
{
}

void GpuOnlyResVulkan::CreateRes(const DeviceVulkan& device, VkBufferUsageFlagBits usageFlag, void* data, uint32_t size)
{
	m_StagingxBuffer.Allocate(device, data, size, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	m_GPUBuffer.Allocate(device, size, VMA_MEMORY_USAGE_GPU_ONLY, usageFlag | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

	BufferTransfer::GetInstance().AddTransferCmd(&m_StagingxBuffer, &m_GPUBuffer, size, 0, 0);
}
