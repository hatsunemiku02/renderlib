

#pragma once
#include "vulkan/vulkan.h"
#include <vector>
class DeviceVulkan;
class CommandPoolVulkan;
class CommadBufferVulkan
{
public:
    CommadBufferVulkan();
    ~CommadBufferVulkan();


    void CreateCommandBuffer(const DeviceVulkan& deviceVulkan,const CommandPoolVulkan& commandPoolVulkan);

    void ResetCommandBuffer()
    {
        vkResetCommandBuffer(m_CommandBuffer, 0);
    }
    
    VkCommandBuffer GetCommandBuffer() const
    {
        return m_CommandBuffer;
    }

    VkCommandBuffer* GetCommandBufferPtr() 
    {
        return &m_CommandBuffer;
    }

private:

    VkCommandBuffer m_CommandBuffer;



};