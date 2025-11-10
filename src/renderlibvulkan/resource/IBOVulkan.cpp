#include "IBOVulkan.h"
#include "../DeviceVulkan.h"

#include <vector>

std::vector<uint16_t> Indices =
{
	 0,1,2
};


void IBOVulkan::CreateIBO(const DeviceVulkan& device, void* data, uint32_t size)
{
	m_IndexBuffer.Allocate(device, data, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

}
