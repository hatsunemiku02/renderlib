#pragma once
#include <vector>
#include "vulkan/vulkan.h"
#include "../CommandPoolVulkan.h"
#include "../CommandBufferVulkan.h"

class BufferVulkan;
class DeviceVulkan;
class QueueVulkan;


struct TransferCmdParam
{
	TransferCmdParam(
		BufferVulkan* psrc,
		BufferVulkan* pdst,
		uint32_t      pbufferSize,
		uint32_t      psrcOffset,
		uint32_t      pdstOffset)
		:src(psrc)
		,dst(pdst)
		,bufferSize(pbufferSize)
		,srcOffset(psrcOffset)
		,dstOffset(pdstOffset)
	{

	}
	BufferVulkan* src;
	BufferVulkan* dst;
	uint32_t      bufferSize;
	uint32_t      srcOffset;
	uint32_t      dstOffset;
};

class BufferTransfer
{
public:
	BufferTransfer(const BufferTransfer&) = delete;
	void operator = (const BufferTransfer&) = delete;
	~BufferTransfer();
	static BufferTransfer& GetInstance() {
		static BufferTransfer instance;
		return instance;
	}
	void Init(const DeviceVulkan& device);
	void AddTransferCmd(BufferVulkan* src,
		BufferVulkan* dst,
		uint32_t      bufferSize,
		uint32_t      srcOffset,
		uint32_t      dstOffset);
	void Excute(const DeviceVulkan& device,const QueueVulkan& queueVulkan);
private:
	BufferTransfer();


	VkFence m_UploadFence;
	CommandPoolVulkan m_CommandPool;
	CommadBufferVulkan m_CommandBuffer;
	std::vector<TransferCmdParam> m_TransferCmdCache;
};

