#pragma once
#include "vulkan/vulkan.h"
#include <memory>
#include <vector>
//#include <shaderc/shaderc.hpp>

class DeviceVulkan;

class ShaderVulkan
{
public:
	ShaderVulkan();
	~ShaderVulkan();


	//std::vector<uint32_t> CompileGLSLToSPIRV(const std::string& source,shaderc_shader_kind kind,const std::string& name);

	void CreateShader(const DeviceVulkan& device, const std::vector<char>& code);

	void DestoryShader(const DeviceVulkan& device);

	const VkShaderModule& GetShaderModule() const
	{
		return m_ShaderModule;
	}

private:



	VkShaderModule m_ShaderModule;
};