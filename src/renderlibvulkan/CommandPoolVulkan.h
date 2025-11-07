

#pragma once
#include "vulkan/vulkan.h"
#include <vector>
class DeviceVulkan;



class CommandPoolVulkan
{
public:
    CommandPoolVulkan();
    ~CommandPoolVulkan();


    void CreateCommandPool(const DeviceVulkan& deviceVulkan);

    VkCommandPool GetCommandPool() const
    {
        return m_CommandPool;
    }
     

private:

    VkCommandPool m_CommandPool;

    

};
