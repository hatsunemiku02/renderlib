#pragma once
#include "vulkan/vulkan.h"
#include <memory>
#include <vector>

class DeviceVulkan;

class ShaderVulkan
{
public:
	ShaderVulkan();
	~ShaderVulkan();

	void CreateShader(const DeviceVulkan& device, const std::vector<char>& code);

	const VkShaderModule& GetShaderModule() const
	{
		return m_ShaderModule;
	}
private:



	VkShaderModule m_ShaderModule;
};