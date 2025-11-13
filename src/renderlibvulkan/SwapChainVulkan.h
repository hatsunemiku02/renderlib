#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <algorithm>
#include "SemaphoreVulkan.h"


class DeviceVulkan;
class RenderpassVulkan;

class SwapChainVulkan
{
public:
   

    //choose best settings for swap chain based on 3 considerations: surf format, presentation mode, swap extent
    void chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, VkSurfaceFormatKHR& format);

    //choose best presentation mode
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    //choose best swap extent; glfw ~[screen units], vulkan ~[pixels]
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int width, int height);


    void createSwapChain(const DeviceVulkan& deviceVulkan, VkSurfaceKHR surface, int width, int height);

    void createImageViews(const DeviceVulkan& deviceVulkan);


    void createFramebuffers(const DeviceVulkan& deviceVulkan,const RenderpassVulkan& passVulkan);

    void createFramebuffers(const DeviceVulkan& deviceVulkan, const VkRenderPass& passVulkan);

    void acquireImageIdx(const DeviceVulkan& deviceVulkan);


    VkFormat GetImgFormat() const
    {
        return swapChainImageFormat;
    }

    VkSwapchainKHR GetSwapchain()
    {
        return swapChain;
    }

    const SemaphoreVulkan& GetimageAvailableSemaphore() const
    {
        return imageAvailableSemaphore;
    }

    VkFramebuffer GetCurrentFrameBuffer() const
    {
        return swapChainFramebuffers[imageIndex];
    }

    uint32_t GetImgIdx() const
    {
        return imageIndex;
    }

    uint32_t*  GetImgIdxAddress() 
    {
        return &imageIndex;
    }

    VkExtent2D GetExtent() const
    {
        return swapChainExtent;
    }
    private:
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        SemaphoreVulkan imageAvailableSemaphore;//sync @gpu
        SemaphoreVulkan renderFinishedSemaphore;//sync @gpu

        uint32_t imageIndex;
};

