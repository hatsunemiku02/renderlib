#include "ShaderVulkan.h"
#include "DeviceVulkan.h"

ShaderVulkan::ShaderVulkan()
{
}

ShaderVulkan::~ShaderVulkan()
{
}

void ShaderVulkan::CreateShader(const DeviceVulkan& device, const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(device.GetDevice(), &createInfo, nullptr, &m_ShaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

}
