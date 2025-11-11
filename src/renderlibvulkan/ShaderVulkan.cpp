#include "ShaderVulkan.h"
#include "DeviceVulkan.h"
#include <stdexcept>

ShaderVulkan::ShaderVulkan()
{
}

ShaderVulkan::~ShaderVulkan()
{
}

//std::vector<uint32_t> ShaderVulkan::CompileGLSLToSPIRV(const std::string& source,
//    shaderc_shader_kind kind,
//    const std::string& name) {
//    shaderc::Compiler compiler;
//    shaderc::CompileOptions options;
//
//    shaderc::SpvCompilationResult result =
//        compiler.CompileGlslToSpv(source.c_str(), source.size(), kind, name.c_str(), options);
//
//    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
//        throw std::runtime_error("Shader compilation failed: " + result.GetErrorMessage());
//    }
//
//    return { result.cbegin(), result.cend() };
//}

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

void ShaderVulkan::DestoryShader(const DeviceVulkan& device)
{
    vkDestroyShaderModule(device.GetDevice(), m_ShaderModule, nullptr);
}
