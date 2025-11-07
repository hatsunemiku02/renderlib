#include "RenderpassVulkan.h"
#include "SwapChainVulkan.h"
#include "DeviceVulkan.h"
#include <algorithm>
#include <stdexcept>

RenderpassVulkan::RenderpassVulkan()
{
}

RenderpassVulkan::~RenderpassVulkan()
{
}

void RenderpassVulkan::DefaultCreate(const DeviceVulkan& device,const SwapChainVulkan& swapChain)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain.GetImgFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    //what to do with the data in the attachment before + after rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;//clears framebuffer to black b4 render
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // N/A don't care about next 2 here
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //images need to be transitioned to specific layouts that are suitable 
    //for the operation that they're going to be involved in next
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //specify the attachment for renderpass and basic subpass referencing the attachment
    //single subpass for simple triangle
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //specify graphics vs compute
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //specify ref to color attachment
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    //subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    //wait for swapchain to finish reading before accessing (by wait for color attch output stage)
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    //prevent transition until ready
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};//VkAttachmentReference reference attachments via indices of this array
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device.GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

}
