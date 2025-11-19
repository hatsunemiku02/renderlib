#include "CommandBufferVulkan.h"
#include "CommandPoolVulkan.h"
#include "DeviceVulkan.h"
#include "RenderpassVulkan.h"
#include "SwapChainVulkan.h"
#include "PipelineVulkan.h"
#include "resource/VBOVulkan.h"
#include "resource/BufferVulkan.h"
#include "resource/TextureVulkan.h"
#include "PipelineLayoutVulkan.h"
#include "DescriptorVulkan.h"
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

void CommadBufferVulkan::BeginCommand(VkCommandBufferUsageFlags usageflag)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usageflag; // Optional
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

void CommadBufferVulkan::BindDescSet(const PipelineLayoutVulkan& pipelineLayout, const DescriptorVulkan& descVulkan)
{
    vkCmdBindDescriptorSets(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout.GetPipelineLayout(), 0, descVulkan.GetDescSetCount(), descVulkan.GetDescSetPtr(),
        0, nullptr);
}

void CommadBufferVulkan::CopyBuffer(const BufferVulkan& src, const BufferVulkan& dst, uint32_t srcoffset, uint32_t dstoffset, uint32_t size)
{
    VkBufferCopy copy;
    copy.dstOffset = dstoffset;
    copy.srcOffset = srcoffset;
    copy.size = size;
    vkCmdCopyBuffer(m_CommandBuffer, src.GetBuffer(), dst.GetBuffer(), 1, &copy);
}

void CommadBufferVulkan::ImageBarrier(const std::vector<TextureVulkan*>& imageList, ImageBarrierType type)
{
    VkImageLayout targetLayout;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkPipelineStageFlagBits srcStageMask;
    VkPipelineStageFlagBits dstStageMask;

    if (type==ToTransfer)
    {
        targetLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        srcAccessMask = 0;
        dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (type == ToRead)
    {
        targetLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        std::runtime_error("Image Barrier Type Not Implemented");
    }

    std::vector<VkImageMemoryBarrier> imgMemBarrier;
    imgMemBarrier.resize(imageList.size());

    for (int i=0;i<imageList.size();i++)
    {
        if (imageList[i]->GetImageLayout() == targetLayout)
        {
            std::runtime_error("new layout equal to old layout,no need to transfer");
        }

        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = 1;
        range.baseArrayLayer = 0;
        range.layerCount = 1;

        VkImageMemoryBarrier imageBarrier_toTransfer = {};
        imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

        imageBarrier_toTransfer.oldLayout = imageList[i]->GetImageLayout();
        imageBarrier_toTransfer.newLayout = targetLayout;
        imageBarrier_toTransfer.image = imageList[i]->GetImage();
        imageBarrier_toTransfer.subresourceRange = range;

        imageBarrier_toTransfer.srcAccessMask = srcAccessMask;
        imageBarrier_toTransfer.dstAccessMask = dstAccessMask;
        imgMemBarrier[i] = imageBarrier_toTransfer;

    }
    //barrier the image into the transfer-receive layout
    vkCmdPipelineBarrier(m_CommandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, imgMemBarrier.size(), imgMemBarrier.data());
}

void CommadBufferVulkan::UploadImageToGPU(const TextureVulkan& image)
{
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;

    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageExtent = image.GetImageSize();

    //copy the buffer into the image
    vkCmdCopyBufferToImage(m_CommandBuffer, image.GetStagingBuffer().GetBuffer(), image.GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}


void CommadBufferVulkan::Draw(uint32_t vtxcount, uint32_t instancecount, uint32_t vtxoffset, uint32_t instanceoffset)
{
    vkCmdDraw(m_CommandBuffer, vtxcount, instancecount, vtxoffset, instanceoffset);
}



