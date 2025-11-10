#include "DeviceVulkan.h"
#include "DynamicRHIVulkan.h"
#include "SwapChainVulkan.h"
#include <vector>
#include <algorithm>
#include <vector>
#include <set>

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

enum class EVulkanQueueType : uint8_t
{
    Graphics = 0,
    AsyncCompute,
    Transfer,

    Count,
};


void PhysicalDeviceFeaturesVulkan::Query(VkPhysicalDevice PhysicalDevice, uint32_t APIVersion)
{

	VkPhysicalDeviceFeatures2 PhysicalDeviceFeatures2;
	ZeroVulkanStruct(PhysicalDeviceFeatures2, VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);

	PhysicalDeviceFeatures2.pNext = &Core_1_1;
	Core_1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;

	if (APIVersion >= VK_API_VERSION_1_2)
	{
		Core_1_1.pNext = &Core_1_2;
		Core_1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	}

	if (APIVersion >= VK_API_VERSION_1_3)
	{
		Core_1_2.pNext = &Core_1_3;
		Core_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	}

	vkGetPhysicalDeviceFeatures2(PhysicalDevice, &PhysicalDeviceFeatures2);

	// Copy features into old struct for convenience
	Core_1_0 = PhysicalDeviceFeatures2.features;

	// Apply config modifications
	//Core_1_0.robustBufferAccess = GCVarRobustBufferAccess.GetValueOnAnyThread() > 0 ? VK_TRUE : VK_FALSE;

	Core_1_0.robustBufferAccess =  VK_FALSE;

	// Apply platform restrictions
	//FVulkanPlatform::RestrictEnabledPhysicalDeviceFeatures(this);

	Core_1_0.shaderResourceResidency = VK_FALSE;
	Core_1_0.shaderResourceMinLod = VK_FALSE;
	Core_1_0.sparseBinding = VK_FALSE;
	Core_1_0.sparseResidencyBuffer = VK_FALSE;
	Core_1_0.sparseResidencyImage2D = VK_FALSE;
	Core_1_0.sparseResidencyImage3D = VK_FALSE;
	Core_1_0.sparseResidency2Samples = VK_FALSE;
	Core_1_0.sparseResidency4Samples = VK_FALSE;
	Core_1_0.sparseResidency8Samples = VK_FALSE;
	Core_1_0.sparseResidencyAliased = VK_FALSE;
}

DeviceVulkan::DeviceVulkan()
{
}

DeviceVulkan::~DeviceVulkan()
{
}

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool DeviceVulkan::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices = DynamicRHIVulkan::GetInstance().FindQueueFamilies(device,surface);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	//check at least 1 supported img format and presentation mode
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport;
		DynamicRHIVulkan::GetInstance().QuerySwapChainSupport(device, surface, swapChainSupport);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool DeviceVulkan::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}


void DeviceVulkan::pickPhysicalDevice(VkSurfaceKHR surface) {

	VkInstance instance = DynamicRHIVulkan::GetInstance().GetVkInstance();
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device, surface)) {
			m_GPU = device;
			break;
		}
	}
	if (m_GPU == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	DynamicRHIVulkan::GetInstance().FindQueueFamilies(m_GPU, surface);
}

template<typename BitsType>
constexpr bool VKHasAllFlags(VkFlags Flags, BitsType Contains)
{
	return (Flags & Contains) == Contains;
}





void DeviceVulkan::CreateDevice()
{

	QueueFamilyIndices indices = DynamicRHIVulkan::GetInstance().GetQueueFamilies();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	//createInfo.enabledExtensionCount = 0;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_GPU, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	//vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
	//vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);


	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = m_GPU;
	allocatorInfo.device = m_Device;
	allocatorInfo.instance = DynamicRHIVulkan::GetInstance().GetVkInstance();
	vmaCreateAllocator(&allocatorInfo, &m_Allocator);
}


