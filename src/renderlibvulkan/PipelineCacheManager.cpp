#include "PipelineCacheManager.h"
#include "DeviceVulkan.h"
#include "DynamicRHIVulkan.h"
#include <stdexcept>
#include <fstream>

PipelineCacheManager::PipelineCacheManager()
{
}


PipelineCacheManager::~PipelineCacheManager()
{
}




bool PipelineCacheManager::CreatePipelineCache(const DeviceVulkan& device) {
    VkPipelineCacheCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    // 尝试从文件加载缓存的初始数据
    std::vector<char> initialData = LoadCacheData();
    if (!initialData.empty()) {
        createInfo.initialDataSize = initialData.size();
        createInfo.pInitialData = initialData.data();
    }
    else {
        createInfo.initialDataSize = 0;
        createInfo.pInitialData = nullptr;
    }

    // 创建管线缓存对象
    VKCHECK(vkCreatePipelineCache(device.GetDevice(), &createInfo, nullptr, &m_cache));

    return true;
}

bool PipelineCacheManager::SaveCacheToFile(const DeviceVulkan& device) {
    size_t dataSize = 0;
    VkResult res = vkGetPipelineCacheData(device.GetDevice(), m_cache, &dataSize, nullptr);
    if (res != VK_SUCCESS || dataSize == 0) {
        std::cerr << "Failed to get cache data size" << std::endl;
        return false;
    }

    std::vector<char> cacheData(dataSize);
    res = vkGetPipelineCacheData(device.GetDevice(), m_cache, &dataSize, cacheData.data());
    if (res != VK_SUCCESS) {
        std::cerr << "Failed to get cache data" << std::endl;
        return false;
    }

    // 写入文件
    std::ofstream file(m_filename, std::ios::binary);
    if (!file.write(cacheData.data(), dataSize)) {
        std::cerr << "Failed to write cache to file" << std::endl;
        return false;
    }
    return true;
}

bool PipelineCacheManager::MergeCaches(const DeviceVulkan& device, const std::vector<VkPipelineCache>& srcCaches) {
    VKCHECK(vkMergePipelineCaches(device.GetDevice(), m_cache, srcCaches.size(), srcCaches.data()));
    return true;
}

void PipelineCacheManager::Destroy(const DeviceVulkan& device) {
    if (m_cache != VK_NULL_HANDLE) {
        vkDestroyPipelineCache(device.GetDevice(), m_cache, nullptr);
        m_cache = VK_NULL_HANDLE;
    }
}

std::vector<char> PipelineCacheManager::LoadCacheData() {
    std::ifstream file(m_filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return {}; // 文件不存在时返回空数据
    }

    size_t fileSize = file.tellg();
    std::vector<char> data(fileSize);
    file.seekg(0);
    file.read(data.data(), fileSize);

    // 验证缓存头兼容性（可选：检查 vendorID/deviceID 等）
    if (fileSize < 16) return {}; // 头部长度不足
    // 实际应用中应校验头部字段（如 vendorID、deviceID）确保设备兼容[5,6](@ref)
    return data;
}