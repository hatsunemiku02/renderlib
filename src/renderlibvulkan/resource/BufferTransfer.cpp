#include "BufferTransfer.h"

#include "BufferVulkan.h"
#include "TextureVulkan.h"

#include "../DeviceVulkan.h"
#include "../QueueVulkan.h"
#include "../SemaphoreVulkan.h"


BufferTransfer::BufferTransfer()
	:m_ImageUploadCmdCache()
	,m_TransferCmdCache()
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

void BufferTransfer::AddUpLoadImgCmd(const TextureVulkan& tex)
{
	m_ImageUploadCmdCache.emplace_back((TextureVulkan*) &tex);
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

	if (m_ImageUploadCmdCache.empty()==false)
	{
		std::vector<TextureVulkan*> imgBarrierList;
		imgBarrierList.resize(m_ImageUploadCmdCache.size());
		for (int i = 0; i < m_ImageUploadCmdCache.size(); i++)
		{
			imgBarrierList[i] = m_ImageUploadCmdCache[i].m_pImage;
		}
		m_CommandBuffer.ImageBarrier(imgBarrierList, CommadBufferVulkan::ToTransfer);
		for (int i = 0; i < m_ImageUploadCmdCache.size(); i++)
		{
			m_CommandBuffer.UploadImageToGPU(*(m_ImageUploadCmdCache[i].m_pImage));
		}
		m_CommandBuffer.ImageBarrier(imgBarrierList, CommadBufferVulkan::ToRead);
	}

	m_CommandBuffer.EndCommand();

	std::vector<CommadBufferVulkan*> cmdlist;
	std::vector<SemaphoreVulkan*> waitsmplist;
	std::vector<SemaphoreVulkan*> signalsmplist;
	cmdlist.push_back(&m_CommandBuffer);
	queueVulkan.Submit(cmdlist, waitsmplist, signalsmplist);

	for (int i = 0; i < m_TransferCmdCache.size(); i++)
	{
		m_TransferCmdCache[i].src->Destory(device);
	}
	for (int i = 0; i < m_ImageUploadCmdCache.size(); i++)
	{
		m_ImageUploadCmdCache[i].m_pImage->DestoryStagingBuffer(device);
	}
	m_TransferCmdCache.clear();
	m_ImageUploadCmdCache.clear();
}
