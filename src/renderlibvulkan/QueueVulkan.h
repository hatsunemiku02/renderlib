#pragma once
#include "vulkan/vulkan.h"


class DeviceVulkan;

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
};



