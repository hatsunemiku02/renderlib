#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <string>

class DeviceVulkan;

class PipelineCacheManager
{
public:
	~PipelineCacheManager();


	static PipelineCacheManager& GetInstance()
	{
		static PipelineCacheManager instance;
		return instance;
	}

    bool CreatePipelineCache(const DeviceVulkan& device);

    // 2. 获取缓存对象（用于管线创建）
    VkPipelineCache GetCache() const {
        return m_cache;
    }

    // 3. 将缓存数据保存到文件（应用退出时调用）
    bool SaveCacheToFile(const DeviceVulkan& device);

    // 4. 合并多个缓存（用于优化或预编译场景）
    bool MergeCaches(const DeviceVulkan& device, const std::vector<VkPipelineCache>& srcCaches);

    // 5. 销毁缓存（应用清理时调用）
    void Destroy(const DeviceVulkan& device);

private:
    // 从文件加载缓存数据
    std::vector<char> LoadCacheData();

    PipelineCacheManager();
    VkPipelineCache m_cache;    // 管线缓存对象
    std::string m_filename;     // 缓存文件路径


};