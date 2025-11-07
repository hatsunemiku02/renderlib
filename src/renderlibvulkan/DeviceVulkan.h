#pragma once

#include "vulkan/vulkan.h"
#include <vector>


template< class T >
static inline void ZeroVulkanStruct(T& Struct, int32_t VkStructureType)
{
	//static_assert(!TIsPointer<T>::Value, "Don't use a pointer!");
	//static_assert(STRUCT_OFFSET(T, sType) == 0, "Assumes sType is the first member in the Vulkan type!");
	//static_assert(sizeof(T::sType) == sizeof(int32), "Assumed sType is compatible with int32!");
	//// Horrible way to coerce the compiler to not have to know what T::sType is so we can have this header not have to include vulkan.h
	//(int32&)Struct.sType = VkStructureType;
	//// FMemory::Memzero(((uint8*)&Struct) + sizeof(VkStructureType), sizeof(T) - sizeof(VkStructureType));
	//
	//memset(((uint8*)&Struct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
}

class PhysicalDeviceFeaturesVulkan
{
public:
	PhysicalDeviceFeaturesVulkan()
	{
		memset(this, 0, sizeof(PhysicalDeviceFeaturesVulkan));
	}

	void Query(VkPhysicalDevice PhysicalDevice, uint32_t APIVersion);

	VkPhysicalDeviceFeatures	     Core_1_0;
	VkPhysicalDeviceVulkan11Features Core_1_1;
private:
	// Anything above Core 1.1 cannot be assumed, they should only be used by the device at init time
	VkPhysicalDeviceVulkan12Features Core_1_2;
	VkPhysicalDeviceVulkan13Features Core_1_3;

	friend class DeviceVulkan;
	//friend DynamicRHIVulkan;
};

class DeviceVulkan
{
public:
	DeviceVulkan();

	~DeviceVulkan();

	void pickPhysicalDevice(VkSurfaceKHR surface);
	

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	void CreateDevice();

	VkDevice GetDevice() const
	{
		return m_Device;
	}

	VkPhysicalDevice GetPhysicalDevice() const
	{
		return m_GPU;
	}

	//VkQueue GetGraphicQueue()
	//{
	//	return m_GraphicsQueue;
	//}
	//
	//VkQueue GetPresentQueue()
	//{
	//	return m_PresentQueue;
	//}

private:

	bool isDeviceSuitable(VkPhysicalDevice device,VkSurfaceKHR surface);

	VkDevice m_Device;
	VkPhysicalDevice m_GPU;

	VkPhysicalDeviceProperties m_GPUProps;

	//VkQueue m_GraphicsQueue;
	//VkQueue m_PresentQueue;

	std::vector<uint32_t> m_ActiveQueueFamilies;
	std::vector<VkQueueFamilyProperties> m_QueueFamilyProps;

	PhysicalDeviceFeaturesVulkan m_PhysicalDeviceFeatures;
};