#include "DescriptorVulkan.h"
#include <stdexcept>
#include <array>
#include "DeviceVulkan.h"
#include "PipelineLayoutVulkan.h"
#include "resource/TextureVulkan.h"



DescriptorVulkan::DescriptorVulkan()
{
}

DescriptorVulkan::~DescriptorVulkan()
{
}

void DescriptorVulkan::CreateDescriptor(const DeviceVulkan& device, const PipelineLayoutVulkan& layout)
{
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(1);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(1);  // 最大描述符集数量


    if (vkCreateDescriptorPool(device.GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    std::vector<VkDescriptorSetLayout> layouts(1, layout.GetDescSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
    allocInfo.pSetLayouts = layouts.data();


    m_DescriptorSets.resize(1);
    if (vkAllocateDescriptorSets(device.GetDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    
}

void DescriptorVulkan::UpdateTexture(const DeviceVulkan& device, const TextureVulkan& texture)
{
    // 图像采样器信息
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture.GetImageView();
    imageInfo.sampler = texture.GetSampler();

    // 描述符写入操作
    std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

    // 组合图像采样器描述符
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = m_DescriptorSets[0];
    descriptorWrites[0].dstBinding = 0;  // 对应着色器中的binding = 1
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pImageInfo = &imageInfo;

    // 更新描述符集
    vkUpdateDescriptorSets(device.GetDevice(), static_cast<uint32_t>(descriptorWrites.size()),
        descriptorWrites.data(), 0, nullptr);
}
