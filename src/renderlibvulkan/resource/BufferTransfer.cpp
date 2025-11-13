#include "BufferTransfer.h"

#include "BufferVulkan.h"

#include "../DeviceVulkan.h"
#include "../QueueVulkan.h"
#include "../SemaphoreVulkan.h"


BufferTransfer::BufferTransfer()
{
}

BufferTransfer::~BufferTransfer()
{
}

void BufferTransfer::Init(const DeviceVulkan& device)
{
	//VkFenceCreateInfo fenceInfo{};
	//fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	//
	//vkCreateFence(device.GetDevice(), &fenceInfo, nullptr, &m_UploadFence);
	m_CommandPool.CreateCommandPool(device);


	m_CommandBuffer.CreateCommandBuffer(device, m_CommandPool);
}

void BufferTransfer::AddTransferCmd(BufferVulkan* src, BufferVulkan* dst, uint32_t bufferSize, uint32_t srcOffset, uint32_t dstOffset)
{
	m_TransferCmdCache.emplace_back(src, dst, bufferSize, srcOffset, dstOffset);
}

void BufferTransfer::Excute(const DeviceVulkan& device,const QueueVulkan& queueVulkan)
{
	if (m_TransferCmdCache.size()==0)
	{
		return;
	}
	queueVulkan.WaitSubmit(device);

	m_CommandBuffer.BeginCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	for (int i=0;i< m_TransferCmdCache.size();i++)
	{
		m_CommandBuffer.CopyBuffer(*m_TransferCmdCache[i].src, *m_TransferCmdCache[i].dst, m_TransferCmdCache[i].srcOffset, m_TransferCmdCache[i].dstOffset, m_TransferCmdCache[i].bufferSize);
	}

	m_CommandBuffer.EndCommand();

	std::vector<CommadBufferVulkan*> cmdlist;
	std::vector<SemaphoreVulkan*> waitsmplist;
	std::vector<SemaphoreVulkan*> signalsmplist;
	cmdlist.push_back(&m_CommandBuffer);
	queueVulkan.Submit(cmdlist, waitsmplist, signalsmplist);

	m_TransferCmdCache.clear();

	//VK_CHECK(vkEndCommandBuffer(cmd));
	//
	//VkSubmitInfo submit = vkinit::submit_info(&cmd);
	//
	//
	////submit command buffer to the queue and execute it.
	//// _uploadFence will now block until the graphic commands finish execution
	//VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, _uploadContext._uploadFence));
	//
	//vkWaitForFences(_device, 1, &_uploadContext._uploadFence, true, 9999999999);
	//vkResetFences(_device, 1, &_uploadContext._uploadFence);
	//
	////clear the command pool. This will free the command buffer too
	//vkResetCommandPool(_device, _uploadContext._commandPool, 0);
}
