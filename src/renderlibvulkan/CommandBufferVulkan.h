

#pragma once
#include "vulkan/vulkan.h"
#include <vector>
class DeviceVulkan;
class CommandPoolVulkan;
class RenderpassVulkan;
class SwapChainVulkan;
class PipelineVulkan;
class VBOVulkan;
class CommadBufferVulkan
{
public:
    CommadBufferVulkan();
    ~CommadBufferVulkan();


    void CreateCommandBuffer(const DeviceVulkan& deviceVulkan,const CommandPoolVulkan& commandPoolVulkan);

    void BeginCommand();

    void EndCommand();

    void BeginRenderPass(const RenderpassVulkan& renderpassVulkan,const SwapChainVulkan& swapchain);

    void EndRenderPass();

    void BindPipeline(const PipelineVulkan& pipline);

    void BindVBO(const VBOVulkan& pipline, uint64_t offset);

    void Draw(uint32_t vtxcount,uint32_t instancecount, uint32_t vtxoffset, uint32_t instanceoffset);

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