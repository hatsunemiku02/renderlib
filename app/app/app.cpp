#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <array>
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
#include "renderlibvulkan/SemaphoreVulkan.h"
#include "renderlibvulkan/resource/BufferTransfer.h"
#include "renderlibvulkan/resource/TextureVulkan.h"


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
TextureVulkan textureVulkan;

std::vector<VkDescriptorSet> descriptorSets;
std::vector<char> imagerbga8;


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
    //VkSemaphore renderFinishedSemaphore;//sync @gpu
    SemaphoreVulkan renderFinishedSemaphore;


    //VkFence inFlightFence;//sync @cpu for next frame

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }//-1


    //START: initialization
    void initVulkan() {

        imagerbga8.resize(16 * 16 * 4);
        for (int i=0;i< imagerbga8.size();i++)
        {
            if (i%4==2||i % 4 == 3)
            {
                imagerbga8[i] = (char)255;
            }
            else
            {
                imagerbga8[i] = 0;
            }
        }

        DynamicRHIVulkan::GetInstance().CreateInstance();
        DynamicRHIVulkan::GetInstance().setupDebugMessenger();
        createSurface();
        deviceVulkan.pickPhysicalDevice(surface);
        deviceVulkan.CreateDevice();

        BufferTransfer::GetInstance().Init(deviceVulkan);

        graphicQueue.Create(EQueueType::Graphics, deviceVulkan);
        presentQueue.Create(EQueueType::Present, deviceVulkan);

        swapchainVulkan.createSwapChain(deviceVulkan, surface, 800, 600);
        swapchainVulkan.createImageViews(deviceVulkan);
        renderpassVulkan.DefaultCreate(deviceVulkan, swapchainVulkan);


        vboVulkan.DefaultCreate(deviceVulkan);
        VkExtent3D imagesize;
        imagesize.width = 16;
        imagesize.height = 16;
        imagesize.depth = 1;
        textureVulkan.CreateTexture(deviceVulkan, imagerbga8.data(), VK_FORMAT_B8G8R8A8_SRGB, imagesize);

        auto vertShaderCode = readFile("D:/workspace/renderlib/res/shader/shader.vert.spv");
        auto fragShaderCode = readFile("D:/workspace/renderlib/res/shader/shader.frag.spv");

        vsShaderVulkan.CreateShader(deviceVulkan, vertShaderCode);
        psShaderVulkan.CreateShader(deviceVulkan, fragShaderCode);

        piplineVulkan.SetShaderBind(vsShaderVulkan, psShaderVulkan);
        piplineVulkan.SetVertexBind(vboVulkan.GetVertexDesc());
        piplineVulkan.CreateGraphicPipeline(deviceVulkan, renderpassVulkan);
        vsShaderVulkan.DestoryShader(deviceVulkan);
        psShaderVulkan.DestoryShader(deviceVulkan);


        std::array<VkDescriptorPoolSize, 1> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(1);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(1);  // 最大描述符集数量

        VkDescriptorPool descriptorPool;
        if (vkCreateDescriptorPool(deviceVulkan.GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        std::vector<VkDescriptorSetLayout> layouts(1, piplineVulkan.GetDescSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
        allocInfo.pSetLayouts = layouts.data();

       
        descriptorSets.resize(1);
        if (vkAllocateDescriptorSets(deviceVulkan.GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        // 更新描述符集
        for (size_t i = 0; i < 1; i++) {
            // Uniform缓冲区信息
   

            // 图像采样器信息
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = textureVulkan.GetImageView();
            imageInfo.sampler = textureVulkan.GetSampler();

            // 描述符写入操作
            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

            // 组合图像采样器描述符
            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;  // 对应着色器中的binding = 1
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pImageInfo = &imageInfo;

            // 更新描述符集
            vkUpdateDescriptorSets(deviceVulkan.GetDevice(), static_cast<uint32_t>(descriptorWrites.size()),
                descriptorWrites.data(), 0, nullptr);
        }



        //createRenderPass();
        

        swapchainVulkan.createFramebuffers(deviceVulkan,renderpassVulkan);

        commandpoolVulkan.CreateCommandPool(deviceVulkan);
        commandbufferVulkan.CreateCommandBuffer(deviceVulkan, commandpoolVulkan);
        createSyncObjects();
    }//0




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
        //VkSemaphoreCreateInfo semaphoreInfo{};
        //semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        //VkFenceCreateInfo fenceInfo{};
        //fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        //fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;//create in signaled state so don't wait on first frame!
        //if (
        //    vkCreateSemaphore(deviceVulkan.GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        //    vkCreateFence(deviceVulkan.GetDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        //    throw std::runtime_error("failed to create semaphores!");
        //}
        renderFinishedSemaphore.CreateSemaphore(deviceVulkan, 0);


    }//13
    //END: initialization

    void drawFrame() {
        //std::cout<<"Tick"<<std::endl;

        BufferTransfer::GetInstance().Excute(deviceVulkan,graphicQueue);

        graphicQueue.WaitSubmit(deviceVulkan);

        swapchainVulkan.acquireImageIdx(deviceVulkan);
        //ready to record the command buffer
        //vkResetCommandBuffer(commandbufferVulkan.GetCommandBuffer(), 0);
        commandbufferVulkan.ResetCommandBuffer();
        //recordCommandBuffer(commandbufferVulkan.GetCommandBuffer(), swapchainVulkan.GetImgIdx());
        commandbufferVulkan.BeginCommand();
        commandbufferVulkan.BeginRenderPass(renderpassVulkan, swapchainVulkan);
        commandbufferVulkan.BindPipeline(piplineVulkan);
        commandbufferVulkan.BindVBO(vboVulkan, 0);

        vkCmdBindDescriptorSets(commandbufferVulkan.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            piplineVulkan.GetPipeLineLayout(), 0, 1, &descriptorSets[0],
            0, nullptr);

        commandbufferVulkan.Draw(3, 1, 0, 0);
        commandbufferVulkan.EndRenderPass();

        commandbufferVulkan.EndCommand();

        std::vector<CommadBufferVulkan*> cmdlist;
        cmdlist.push_back(&commandbufferVulkan);
        std::vector<SemaphoreVulkan*> waitsmplist;
        waitsmplist.push_back(const_cast<SemaphoreVulkan*>(&swapchainVulkan.GetimageAvailableSemaphore()) );
        std::vector<SemaphoreVulkan*> signalsmplist;
        signalsmplist.push_back(&renderFinishedSemaphore);
        graphicQueue.Submit(cmdlist, waitsmplist, signalsmplist);



        //submit the result back to swapchain for presentation
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.GetSemaphore()};
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