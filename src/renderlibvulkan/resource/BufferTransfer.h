#pragma once
#include <vector>
#include "vulkan/vulkan.h"
#include "../CommandPoolVulkan.h"
#include "../CommandBufferVulkan.h"

class BufferVulkan;
class DeviceVulkan;
class QueueVulkan;
class TextureVulkan;

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


//VkBufferImageCopy copyRegion = {};
//copyRegion.bufferOffset = 0;
//copyRegion.bufferRowLength = 0;
//copyRegion.bufferImageHeight = 0;
//
//copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//copyRegion.imageSubresource.mipLevel = 0;
//copyRegion.imageSubresource.baseArrayLayer = 0;
//copyRegion.imageSubresource.layerCount = 1;
//copyRegion.imageExtent = imageExtent;

struct ImageUploadCmdParam
{
	ImageUploadCmdParam( TextureVulkan* img)
		:m_pImage(img)
	{

	}
	TextureVulkan* m_pImage;
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
	void AddUpLoadImgCmd(const TextureVulkan& tex);
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
	std::vector<ImageUploadCmdParam> m_ImageUploadCmdCache;
};

