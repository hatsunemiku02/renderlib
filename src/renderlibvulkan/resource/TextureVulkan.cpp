#include <stdexcept>
#include "TextureVulkan.h"
#include "BufferTransfer.h"
#include "../DeviceVulkan.h"

TextureVulkan::TextureVulkan()
{

}

TextureVulkan::~TextureVulkan()
{

}

void TextureVulkan::CreateTexture(const DeviceVulkan& device, void* data, VkFormat imageFormat, VkExtent3D imageSize)
{

	uint32_t size = 0;
	size = imageSize.width * imageSize.height * imageSize.depth;
	uint32_t pixelsize = 0;
	switch (imageFormat)
	{
	case VK_FORMAT_B8G8R8A8_SRGB:
		pixelsize = 4;
		break;
	default:
		std::runtime_error("unknown image format to caculte img size");
		break;
	}
	size = size * pixelsize;

	m_StagingBuffer.Allocate(device, data, size, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	VkImageCreateInfo imgCreateImfo = {};
	imgCreateImfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgCreateImfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	imgCreateImfo.imageType = VK_IMAGE_TYPE_2D;
	imgCreateImfo.format = imageFormat  ;
	imgCreateImfo.extent = imageSize;
	imgCreateImfo.mipLevels = 1;
	imgCreateImfo.arrayLayers = 1;
	imgCreateImfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imgCreateImfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateImfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imgCreateImfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VmaAllocationCreateInfo vmaallocInfo{};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	vmaCreateImage(device.GetAllocator(), &imgCreateImfo, &vmaallocInfo, &m_Image, &m_Allocation,nullptr );

	m_CurrentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_ImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	m_ImageType = VK_IMAGE_TYPE_2D;
	m_ImageExtent = imageSize;

	BufferTransfer::GetInstance().AddUpLoadImgCmd(*this);



	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;  // 放大过滤
	samplerInfo.minFilter = VK_FILTER_LINEAR;  // 缩小过滤
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;  // U方向寻址模式
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;  // V方向寻址模式
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;  // W方向寻址模式
	samplerInfo.anisotropyEnable = VK_TRUE;  // 启用各向异性过滤

	// 查询物理设备属性，获取最大各向异性采样数
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device.GetPhysicalDevice(), &properties);
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;  // 使用归一化坐标[0,1)
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;  // Mipmap模式
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	
	if (vkCreateSampler(device.GetDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}


	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_Image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = imageFormat;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device.GetDevice(), &viewInfo, nullptr, &m_ImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}
