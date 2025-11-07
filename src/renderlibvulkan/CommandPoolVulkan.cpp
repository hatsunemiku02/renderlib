#include "CommandPoolVulkan.h"
#include "DynamicRHIVulkan.h"
#include "DeviceVulkan.h"


CommandPoolVulkan::CommandPoolVulkan()
{
}

CommandPoolVulkan::~CommandPoolVulkan()
{
}

void CommandPoolVulkan::CreateCommandPool(const DeviceVulkan& deviceVulkan)
{
    QueueFamilyIndices queueFamilyIndices = DynamicRHIVulkan::GetInstance().GetQueueFamilies();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //this prog records cmd bffr every frame, so want rerecording indiv
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;//allow re-recording individually
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    //draw cmds req cmd buffer get submitted to graphics queue
    if (vkCreateCommandPool(deviceVulkan.GetDevice(), &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}