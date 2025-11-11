#include "CommandBufferVulkan.h"
#include "CommandPoolVulkan.h"
#include "DeviceVulkan.h"
#include "RenderpassVulkan.h"
#include "SwapChainVulkan.h"
#include "PipelineVulkan.h"
#include "resource/VBOVulkan.h"
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

void CommadBufferVulkan::BeginCommand()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void CommadBufferVulkan::EndCommand()
{
    if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void CommadBufferVulkan::BeginRenderPass(const RenderpassVulkan& renderpassVulkan, const SwapChainVulkan& swapchain)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderpassVulkan.GetRenderpass();
    renderPassInfo.framebuffer = swapchain.GetCurrentFrameBuffer();//attachments to bind (here = color attch)
    renderPassInfo.renderArea.offset = { 0, 0 };//size of render area where shader loads+stores take place
    renderPassInfo.renderArea.extent = swapchain.GetExtent();//match attch size for best perf
    VkClearValue clearColor = { {{0.2f, 0.2f, 0.4f, 1.0f}} };//osg bkg
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    //set viewport and scissor b/c opted for dynamic
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain.GetExtent().width);
    viewport.height = static_cast<float>(swapchain.GetExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchain.GetExtent();
    vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);
}

void CommadBufferVulkan::EndRenderPass()
{
    vkCmdEndRenderPass(m_CommandBuffer);
}

void CommadBufferVulkan::BindPipeline(const PipelineVulkan& pipline)
{
    VkPipelineBindPoint bindpoint;
    switch (pipline.GetPipelineType())
    {
    case PipelineVulkan::Graphic:
        bindpoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        break;
    case  PipelineVulkan::Compute:
        bindpoint = VK_PIPELINE_BIND_POINT_COMPUTE;
        break;
    default:
        break;
    }
    vkCmdBindPipeline(m_CommandBuffer, bindpoint, pipline.GetPipeline());
}

void CommadBufferVulkan::BindVBO(const VBOVulkan& vbo, uint64_t offset)
{
    uint64_t offsetin = offset;
    vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &vbo.GetBuffer().GetBuffer(), &offsetin);
}

void CommadBufferVulkan::Draw(uint32_t vtxcount, uint32_t instancecount, uint32_t vtxoffset, uint32_t instanceoffset)
{
    vkCmdDraw(m_CommandBuffer, vtxcount, instancecount, vtxoffset, instanceoffset);
}



