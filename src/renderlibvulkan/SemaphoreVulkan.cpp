#include "SemaphoreVulkan.h"
#include "DeviceVulkan.h"
#include "DynamicRHIVulkan.h"

SemaphoreVulkan::SemaphoreVulkan()
	:m_Semaphore()
	, m_StageFlag()
{

}

SemaphoreVulkan::~SemaphoreVulkan()
{

}

void SemaphoreVulkan::CreateSemaphore(const DeviceVulkan& device, VkPipelineStageFlags flag)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VKCHECK(vkCreateSemaphore(device.GetDevice(), &semaphoreInfo, nullptr, &m_Semaphore));
    m_StageFlag = flag;
}
