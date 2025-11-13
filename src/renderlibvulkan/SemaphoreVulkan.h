#pragma once
#include "vulkan/vulkan.h"


class DeviceVulkan;
class SemaphoreVulkan
{
public:
	SemaphoreVulkan();
	~SemaphoreVulkan();


	void CreateSemaphore(const DeviceVulkan& device, VkPipelineStageFlags flag);
	void SetStageFlag(VkPipelineStageFlags flag)
	{
		m_StageFlag = flag;
	}
	VkPipelineStageFlags GetStageFlag() const
	{
		return m_StageFlag;
	}
	VkSemaphore GetSemaphore() const
	{
		return m_Semaphore;
	}
private:

	VkSemaphore m_Semaphore;

	VkPipelineStageFlags m_StageFlag;

};