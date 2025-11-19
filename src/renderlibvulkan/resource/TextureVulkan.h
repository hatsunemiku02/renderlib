#pragma once

#include <vector>
#include <memory>
#include "BufferVulkan.h"
#include "vk_mem_alloc.h"

class DeviceVulkan;

class TextureVulkan
{
public:
	TextureVulkan();
	~TextureVulkan();
	void CreateTexture(const DeviceVulkan& device, void* data, VkFormat imageFormat, VkExtent3D imageSize);

	VkImage GetImage() const
	{
		return m_Image;
	}
	VkImageLayout GetImageLayout() const
	{
		return m_CurrentLayout;
	}
	VkFormat GetImageFormat() const
	{
		return m_ImageFormat;
	}
	VkImageType GetImageType() const
	{
		return m_ImageType;
	}
	const VkExtent3D& GetImageSize() const
	{
		return m_ImageExtent;
	}
	const BufferVulkan& GetStagingBuffer() const
	{
		return m_StagingBuffer;
	}

	void DestoryStagingBuffer(const DeviceVulkan& device)
	{
		m_StagingBuffer.Destory(device);
	}

	VkImageView GetImageView() const
	{
		return m_ImageView;
	}

	VkSampler GetSampler() const
	{
		return m_Sampler;
	}
private:
	BufferVulkan  m_StagingBuffer;
	VkImage		  m_Image;
	VmaAllocation m_Allocation;
	VkImageLayout m_CurrentLayout;
	VkFormat      m_ImageFormat;
	VkImageType   m_ImageType;
	VkExtent3D    m_ImageExtent;

	VkImageView   m_ImageView;
	VkSampler     m_Sampler;
};