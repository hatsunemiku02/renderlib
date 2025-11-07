#include "CommandBufferVulkan.h"
#include "CommandPoolVulkan.h"
#include "DeviceVulkan.h"
#include <stdexcept>

CommadBufferVulkan::CommadBufferVulkan()
{
}

CommadBufferVulkan::~CommadBufferVulkan()
{
}

void CommadBufferVulkan::CreateCommandBuffer(const DeviceVulkan& deviceVulkan,const CommandPoolVulkan& commandPoolVulkan) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPoolVulkan.GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(deviceVulkan.GetDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

