// Copyright Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	VulkanDynamicRHI.h: Vulkan RHI definitions.
=============================================================================*/

#pragma once
#include <string>
#include <stdexcept>
#include <optional>
#include <iostream>
#include "IDynamicRHIVulkan.h"



#ifdef NDEBUG
static const bool enableValidationLayers = false;
#else
static const bool enableValidationLayers = true;
#endif

static const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};


struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


#if RHI_NEW_GPU_PROFILER
// Encapsulates the state required for tracking GPU queue performance across a frame.
class FVulkanTiming
{
public:
	FVulkanTiming(FVulkanQueue& InQueue);

	FVulkanQueue& Queue;

	// Timer calibration data
	uint64 GPUFrequency = 0, GPUTimestamp = 0;
	uint64 CPUFrequency = 0, CPUTimestamp = 0;

	UE::RHI::GPUProfiler::FEventStream EventStream;
};
#endif

struct QueueFamilyIndices {
	//ensure both graphics and present capability by the queue we choose
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> computeFamily;
	std::optional<uint32_t> transferFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value()&& computeFamily.has_value() && transferFamily.has_value();
	}
};


/** The interface which is implemented by the dynamically bound RHI. */
class DynamicRHIVulkan 
{
public:

	static DynamicRHIVulkan& GetInstance() {
		static DynamicRHIVulkan instance;
		return instance;
	}

	/** Initialization constructor. */
	DynamicRHIVulkan();

	/** Destructor */
	~DynamicRHIVulkan();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	QueueFamilyIndices GetQueueFamilies();

	
	void QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface, SwapChainSupportDetails& details);

	bool checkValidationLayerSupport();

#if PLATFORM_SUPPORTS_BINDLESS_RENDERING
	virtual FRHIResourceCollectionRef RHICreateResourceCollection(FRHICommandListBase& RHICmdList, TConstArrayView<FRHIResourceCollectionMember> InMembers) final;
#endif


	uint32_t GetApiVersion() const
	{
		return ApiVersion;
	}

	// Runs code on SubmissionThread with access to VkQueue.  Useful for plugins. 
	//virtual void RHIRunOnQueue(EVulkanRHIRunOnQueueType QueueType, TFunction<void(VkQueue)>&& CodeToRun, bool bWaitForSubmission) final override;

	void DumpCrashMarkers();

	static void SavePipelineCache();


	VkInstance GetVkInstance()
	{
		return m_Instance;
	}

	void CreateInstance();


	VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void setupDebugMessenger();

protected:


	// Used when a dedicated interrupt thread is disabled (D3D12_USE_INTERRUPT_THREAD)
	// Tracks the thread ID of the thread currently acting as the interrupt thread.
	uint32_t InterruptThreadID = 0;

	uint32_t ApiVersion = 0;
	VkInstance Instance;
	std::vector<const char*> InstanceExtensions;
	std::vector<const char*> InstanceLayers;

	VkDebugUtilsMessengerEXT debugMessenger;

	bool m_QueueGeted;
	QueueFamilyIndices m_QueueFamilyIndicesCache;

	friend class FVulkanCommandListContext;

	friend class FVulkanViewport;


#if VULKAN_SUPPORTS_VALIDATION_CACHE
	IConsoleObject* SaveValidationCacheCmd = nullptr;
#endif

	static void RebuildPipelineCache();
#if VULKAN_SUPPORTS_VALIDATION_CACHE
	static void SaveValidationCache();
#endif

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	IConsoleObject* DumpMemoryCmd = nullptr;
	IConsoleObject* DumpMemoryFullCmd = nullptr;
	IConsoleObject* DumpStagingMemoryCmd = nullptr;
	IConsoleObject* DumpLRUCmd = nullptr;
	IConsoleObject* TrimLRUCmd = nullptr;
public:
	static void DumpMemory();
	static void DumpMemoryFull();
	static void DumpStagingMemory();
	static void DumpLRU();
	static void TrimLRU();
#endif

public:
	enum class EActiveDebugLayerExtension
	{
		None,
		GfxReconstructLayer,
		VkTraceLayer,
		DebugUtilsExtension
	};

protected:

	VkInstance m_Instance;

	QueueFamilyIndices m_Indices;

	bool bIsStandaloneStereoDevice = false;

	EActiveDebugLayerExtension ActiveDebugLayerExtension = EActiveDebugLayerExtension::None;

#if VULKAN_HAS_DEBUGGING_ENABLED
	VkDebugUtilsMessengerEXT Messenger = VK_NULL_HANDLE;

	//void SetupDebugLayerCallback();
	//void RemoveDebugLayerCallback();
#endif

	

public:

};




