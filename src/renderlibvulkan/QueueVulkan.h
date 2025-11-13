#pragma once
#include "vulkan/vulkan.h"
#include <vector>

class DeviceVulkan;
class SemaphoreVulkan;
class CommadBufferVulkan;

enum class EQueueType : uint8_t
{
	Graphics = 0,
	AsyncCompute,
	Transfer,
	Present,
	Count,
};

class QueueVulkan
{
public:
	QueueVulkan();
	~QueueVulkan();

	void Create(EQueueType type, const DeviceVulkan& device);

	void Submit(const std::vector<CommadBufferVulkan*>& cmdBufferList, const std::vector<SemaphoreVulkan*>& waitSemaphores, const std::vector<SemaphoreVulkan*>& signalSemaphores) const;

	void WaitSubmit(const DeviceVulkan& device) const;

	EQueueType	GetType() const
	{
		return m_QueueType;
	}

	VkQueue GetQueue() const
	{
		return m_Queue;
	}

private:
	EQueueType m_QueueType;
	VkQueue m_Queue;
	VkFence m_SubmitFence;
};



