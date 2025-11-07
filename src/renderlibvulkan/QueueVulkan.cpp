#include "QueueVulkan.h"
#include "DeviceVulkan.h"
#include "DynamicRHIVulkan.h"

QueueVulkan::QueueVulkan()
	:m_QueueType(EQueueType::Count)
{
}

QueueVulkan::~QueueVulkan()
{
}

void QueueVulkan::Create(EQueueType type,const DeviceVulkan& device)
{
	m_QueueType = type;

	uint32_t value = 0;

	QueueFamilyIndices indices = DynamicRHIVulkan::GetInstance().GetQueueFamilies();
	switch (type) {
	case EQueueType::Graphics:
		value = indices.graphicsFamily.value();
	case EQueueType::AsyncCompute:
		value = indices.computeFamily.value();
	case EQueueType::Transfer:
		value = indices.transferFamily.value();
	case EQueueType::Present:
		value = indices.presentFamily.value();
	}
	
	vkGetDeviceQueue(device.GetDevice(), value, 0, &m_Queue);
}
