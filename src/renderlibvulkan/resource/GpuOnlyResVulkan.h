#include "BufferVulkan.h"
#include <vector>
#include <memory>

class DeviceVulkan;


class GpuOnlyResVulkan
{
public:
	GpuOnlyResVulkan();
	~GpuOnlyResVulkan();
	void CreateRes(const DeviceVulkan& device, VkBufferUsageFlagBits usageFlag, void* data, uint32_t size);
	const BufferVulkan&  GetBuffer() const
	{
		return m_GPUBuffer;
	}
private:
	BufferVulkan m_StagingxBuffer;
	BufferVulkan m_GPUBuffer;


};