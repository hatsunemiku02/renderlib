#include "QueueVulkan.h"
#include "DeviceVulkan.h"
#include "DynamicRHIVulkan.h"
#include "CommandBufferVulkan.h"
#include "SemaphoreVulkan.h"

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

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;//create in signaled state so don't wait on first frame!
  
    VKCHECK(vkCreateFence(device.GetDevice(), &fenceInfo, nullptr, &m_SubmitFence));
}

void QueueVulkan::Submit(const std::vector<CommadBufferVulkan*>& cmdBufferList, const std::vector<SemaphoreVulkan*>& waitSemaphores, const std::vector<SemaphoreVulkan*>& signalSemaphores ) const
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::vector <VkPipelineStageFlags> waitStages;//[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    std::vector<VkSemaphore> waitsaplist;
    waitsaplist.resize(waitSemaphores.size());
    waitStages.resize(waitSemaphores.size());
    for (int i=0;i<waitSemaphores.size();i++)
    {
        waitsaplist[i] = waitSemaphores[i]->GetSemaphore();
        waitStages[i] = waitSemaphores[i]->GetStageFlag();
    }

    submitInfo.waitSemaphoreCount = waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitsaplist.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    //specify which cmd buffers to submit (we only have 1)
    submitInfo.commandBufferCount = cmdBufferList.size();
    std::vector<VkCommandBuffer> vkcmdlist;
    vkcmdlist.resize(cmdBufferList.size());
    for (int i=0;i< cmdBufferList.size();i++)
    {
        vkcmdlist[i] = cmdBufferList[i]->GetCommandBuffer();
    }
    submitInfo.pCommandBuffers = vkcmdlist.data();
    //specify which semaphores to signal when cmd finished
    //VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    std::vector<VkSemaphore> signalsaplist;
    signalsaplist.resize(signalSemaphores.size());
    for (int i=0;i< signalSemaphores.size();i++)
    {
        signalsaplist[i] = signalSemaphores[i]->GetSemaphore();
    }
    submitInfo.signalSemaphoreCount = signalsaplist.size();
    submitInfo.pSignalSemaphores = signalsaplist.data();
    //submit cmd buffer to graphics queue
    VKCHECK(vkQueueSubmit(m_Queue, 1, &submitInfo, m_SubmitFence));
       
}

void QueueVulkan::WaitSubmit(const DeviceVulkan& device) const
{
    vkWaitForFences(device.GetDevice(), 1, &m_SubmitFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device.GetDevice(), 1, &m_SubmitFence);
}
