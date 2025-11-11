#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <optional>
#include <set>

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

#include "renderlibvulkan/DynamicRHIVulkan.h"
#include "renderlibvulkan/DeviceVulkan.h"
#include "renderlibvulkan/SwapChainVulkan.h"
#include "renderlibvulkan/RenderpassVulkan.h"
#include "renderlibvulkan/PipelineVulkan.h"
#include "renderlibvulkan/CommandPoolVulkan.h"
#include "renderlibvulkan/CommandBufferVulkan.h"
#include "renderlibvulkan/QueueVulkan.h"
#include "renderlibvulkan/resource/VBOVulkan.h"
#include "renderlibvulkan/ShaderVulkan.h"


DeviceVulkan  deviceVulkan;
SwapChainVulkan swapchainVulkan;
RenderpassVulkan renderpassVulkan;
PipelineVulkan piplineVulkan;
CommandPoolVulkan commandpoolVulkan;
CommadBufferVulkan commandbufferVulkan;
QueueVulkan graphicQueue;
QueueVulkan presentQueue;
VBOVulkan    vboVulkan;
ShaderVulkan vsShaderVulkan;
ShaderVulkan psShaderVulkan;


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;



void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkSurfaceKHR surface;
    VkSemaphore renderFinishedSemaphore;//sync @gpu
    VkFence inFlightFence;//sync @cpu for next frame

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }//-1


    //START: initialization
    void initVulkan() {
        //createInstance();
        DynamicRHIVulkan::GetInstance().CreateInstance();
        DynamicRHIVulkan::GetInstance().setupDebugMessenger();
        createSurface();
        deviceVulkan.pickPhysicalDevice(surface);
        deviceVulkan.CreateDevice();

        graphicQueue.Create(EQueueType::Graphics, deviceVulkan);
        presentQueue.Create(EQueueType::Present, deviceVulkan);

        swapchainVulkan.createSwapChain(deviceVulkan, surface, 800, 600);
        swapchainVulkan.createImageViews(deviceVulkan);
        renderpassVulkan.DefaultCreate(deviceVulkan, swapchainVulkan);


        vboVulkan.DefaultCreate(deviceVulkan);
        //createRenderPass();
        auto vertShaderCode = readFile("D:/workspace/renderlib/res/shader/shader.vert.spv");
        auto fragShaderCode = readFile("D:/workspace/renderlib/res/shader/shader.frag.spv");

        vsShaderVulkan.CreateShader(deviceVulkan, vertShaderCode);
        psShaderVulkan.CreateShader(deviceVulkan, fragShaderCode);

        piplineVulkan.SetShaderBind(vsShaderVulkan, psShaderVulkan);
        piplineVulkan.SetVertexBind(vboVulkan.GetVertexDesc());
        piplineVulkan.CreateGraphicPipeline(deviceVulkan,renderpassVulkan);
        vsShaderVulkan.DestoryShader(deviceVulkan);
        psShaderVulkan.DestoryShader(deviceVulkan);

        swapchainVulkan.createFramebuffers(deviceVulkan,renderpassVulkan);

        commandpoolVulkan.CreateCommandPool(deviceVulkan);
        commandbufferVulkan.CreateCommandBuffer(deviceVulkan, commandpoolVulkan);
        createSyncObjects();
    }//0

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }//1a
    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }//1b




    void createSurface() {
        if (glfwCreateWindowSurface(DynamicRHIVulkan::GetInstance().GetVkInstance(), window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }//3

    //START: graphics pipeline
    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }//9a

    void createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;//create in signaled state so don't wait on first frame!
        if (
            vkCreateSemaphore(deviceVulkan.GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(deviceVulkan.GetDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }//13
    //END: initialization

    //this writes cmds we want to exec to cmd buffer

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
       

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        //start render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderpassVulkan.GetRenderpass();
        renderPassInfo.framebuffer = swapchainVulkan.GetCurrentFrameBuffer();//attachments to bind (here = color attch)
        renderPassInfo.renderArea.offset = { 0, 0 };//size of render area where shader loads+stores take place
        renderPassInfo.renderArea.extent = swapchainVulkan.GetExtent();//match attch size for best perf
        VkClearValue clearColor = { {{0.2f, 0.2f, 0.4f, 1.0f}} };//osg bkg
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        //set viewport and scissor b/c opted for dynamic
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapchainVulkan.GetExtent().width);
        viewport.height = static_cast<float>(swapchainVulkan.GetExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapchainVulkan.GetExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, piplineVulkan.GetPipeline());

        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vboVulkan.GetBuffer().GetBuffer(), &offset);

        //draw that motherfucker
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        //end the render pass
        vkCmdEndRenderPass(commandBuffer);

        //finished recording cmd buffer
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }//14c
    void drawFrame() {
        //std::cout<<"Tick"<<std::endl;
        vkWaitForFences(deviceVulkan.GetDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(deviceVulkan.GetDevice(), 1, &inFlightFence);

        swapchainVulkan.acquireImageIdx(deviceVulkan);
        //ready to record the command buffer
        //vkResetCommandBuffer(commandbufferVulkan.GetCommandBuffer(), 0);
        commandbufferVulkan.ResetCommandBuffer();
        recordCommandBuffer(commandbufferVulkan.GetCommandBuffer(), swapchainVulkan.GetImgIdx());

        //submit cmd buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        //specify which semaphores to wait for
        VkSemaphore waitSemaphores[] = { swapchainVulkan.GetimageAvailableSemaphore() };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        //specify which cmd buffers to submit (we only have 1)
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandbufferVulkan.GetCommandBufferPtr();
        //specify which semaphores to signal when cmd finished
        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        //submit cmd buffer to graphics queue
        if (vkQueueSubmit(graphicQueue.GetQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        //submit the result back to swapchain for presentation
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapchainVulkan.GetSwapchain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = swapchainVulkan.GetImgIdxAddress();

        presentInfo.pResults = nullptr; // Optional
        vkQueuePresentKHR(presentQueue.GetQueue(), &presentInfo);//the line we've been looking for
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(deviceVulkan.GetDevice());
    }

    void cleanup() {

        //vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
        //vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
        //vkDestroyFence(device, inFlightFence, nullptr);
        //
        //vkDestroyCommandPool(device, commandPool, nullptr);
        //for (auto framebuffer : swapChainFramebuffers) {
        //    vkDestroyFramebuffer(device, framebuffer, nullptr);
        //}
        //vkDestroyPipeline(device, graphicsPipeline, nullptr);
        //vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        //vkDestroyRenderPass(device, renderPass, nullptr);
        //if (enableValidationLayers) {
        //    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        //}
        //for (auto imageView : swapChainImageViews) {
        //    vkDestroyImageView(device, imageView, nullptr);
        //}
        //vkDestroySwapchainKHR(device, swapChain, nullptr);
        //vkDestroyDevice(device, nullptr);
        //vkDestroySurfaceKHR(instance, surface, nullptr);
        //vkDestroyInstance(instance, nullptr);
        //glfwDestroyWindow(window);
        //glfwTerminate();
    }//15
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}