// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include <vector>
#include <cstdint>
#include "vulkan/vulkan.h"
//#include "RHI.h"
//#include "VulkanThirdParty.h"
//
//struct FVulkanRHIAllocationInfo
//{
//	VkDeviceMemory Handle{};
//	uint64 Offset{};
//	uint64 Size{};
//};
//
//struct FVulkanRHIImageViewInfo
//{
//	VkImageView ImageView;
//	VkImage Image;
//	VkImageSubresourceRange SubresourceRange;
//	VkFormat Format;
//	uint32 Width;
//	uint32 Height;
//	uint32 Depth;
//	ETextureCreateFlags UEFlags;
//};

struct FVulkanRHIExternalImageDeleteCallbackInfo
{
	void* UserData = nullptr;
	void (*Function)(void* UserData) = nullptr;
};

enum class EVulkanRHIRunOnQueueType
{
	Graphics = 0,
	Transfer,
};

struct IDynamicRHIVulkan
{
	//virtual ERHIInterfaceType GetInterfaceType() const override { return ERHIInterfaceType::Vulkan; }

	virtual uint32_t           RHIGetVulkanVersion() const = 0;
	virtual VkInstance       RHIGetVkInstance() const = 0;
	virtual VkDevice         RHIGetVkDevice() const = 0;
	virtual const uint8_t* RHIGetVulkanDeviceUUID() const = 0;
	virtual VkPhysicalDevice RHIGetVkPhysicalDevice() const = 0;
	virtual const VkAllocationCallbacks* RHIGetVkAllocationCallbacks() = 0;

	virtual VkQueue          RHIGetGraphicsVkQueue() const = 0;
	virtual uint32_t           RHIGetGraphicsQueueIndex() const = 0;
	virtual uint32_t           RHIGetGraphicsQueueFamilyIndex() const = 0;

	virtual VkCommandBuffer  RHIGetActiveVkCommandBuffer() = 0;

	virtual uint64_t    RHIGetGraphicsAdapterLUID(VkPhysicalDevice InPhysicalDevice) const = 0;
	virtual bool             RHIDoesAdapterMatchDevice(const void* InAdapterId) const = 0;
	virtual void* RHIGetVkDeviceProcAddr(const char* InName) const = 0;
	virtual void* RHIGetVkInstanceProcAddr(const char* InName) const = 0;
	/**
	 * Version of RHIGetVkInstanceProcAddr that uses nullptr as the instance argument.
	 * See vkGetInstanceProcAddr manpage for distinction between "global" and non-global commands.
	 */
	virtual void* RHIGetVkInstanceGlobalProcAddr(const char* InName) const = 0;
	//virtual VkFormat         RHIGetSwapChainVkFormat(EPixelFormat InFormat) const = 0;
	virtual bool             RHISupportsEXTFragmentDensityMap2() const = 0;

	virtual std::vector<VkExtensionProperties> RHIGetAllInstanceExtensions() const = 0;
	virtual std::vector<VkExtensionProperties> RHIGetAllDeviceExtensions(VkPhysicalDevice InPhysicalDevice) const = 0;
	//virtual std::vector<FAnsiString> RHIGetLoadedDeviceExtensions() const = 0;

	//virtual FTextureRHIRef   RHICreateTexture2DFromResource(EPixelFormat Format, uint32 SizeX, uint32 SizeY, uint32 NumMips, uint32 NumSamples, VkImage Resource, ETextureCreateFlags Flags, const FClearValueBinding& ClearValueBinding = FClearValueBinding::Transparent, const FVulkanRHIExternalImageDeleteCallbackInfo& ExternalImageDeleteCallbackInfo = {}) = 0;

	//virtual FTextureRHIRef   RHICreateTexture2DArrayFromResource(EPixelFormat Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, VkImage Resource, ETextureCreateFlags Flags, const FClearValueBinding& ClearValueBinding = FClearValueBinding::Transparent) = 0;
	//virtual FTextureRHIRef   RHICreateTextureCubeFromResource(EPixelFormat Format, uint32 Size, bool bArray, uint32 ArraySize, uint32 NumMips, VkImage Resource, ETextureCreateFlags Flags, const FClearValueBinding& ClearValueBinding = FClearValueBinding::Transparent) = 0;

	//virtual VkImage                  RHIGetVkImage(FRHITexture* InTexture) const = 0;
	//virtual VkFormat                 RHIGetViewVkFormat(FRHITexture* InTexture) const = 0;
	//virtual FVulkanRHIAllocationInfo RHIGetAllocationInfo(FRHITexture* InTexture) const = 0;
	//virtual FVulkanRHIImageViewInfo  RHIGetImageViewInfo(FRHITexture* InTexture) const = 0;
	//virtual FVulkanRHIAllocationInfo RHIGetAllocationInfo(FRHIBuffer* InBuffer) const = 0;

	virtual void           RHISetImageLayout(VkImage Image, VkImageLayout OldLayout, VkImageLayout NewLayout, const VkImageSubresourceRange& SubresourceRange) = 0;

	
	virtual void           RHIFinishExternalComputeWork(VkCommandBuffer InCommandBuffer) = 0;
	virtual void           RHIRegisterWork(uint32_t NumPrimitives) = 0;
	virtual void           RHISubmitUploadCommandBuffer() = 0;



	virtual void RHIVerifyResult(VkResult Result, const char* VkFuntion, const char* Filename, uint32_t Line) = 0;

	// FDynamicRHI interface.
	virtual void Init() = 0;
	virtual void PostInit()  = 0;
	virtual void Shutdown()  = 0;
	virtual const char* GetName() = 0;

	static void AddEnabledInstanceExtensionsAndLayers(std::vector<const char* const> InInstanceExtensions, std::vector<const char* const> InInstanceLayers);
	static void AddEnabledDeviceExtensionsAndLayers(std::vector<const char* const> InDeviceExtensions, std::vector<const char* const> InDeviceLayers);

	// Runs code on SubmissionThread with access to VkQueue.  Useful for plugins. 
	//virtual void RHIRunOnQueue(EVulkanRHIRunOnQueueType QueueType, TFunction<void(VkQueue)>&& CodeToRun, bool bWaitForSubmission) = 0;
};

//inline DynamicRHIVulkan* GetDynamicRHIVulkan()
//{
//	checkf(GDynamicRHI, TEXT("Tried to fetch RHI too early"));
//	check(GDynamicRHI->GetInterfaceType() == ERHIInterfaceType::Vulkan);
//	return GetDynamicRHI<DynamicRHIVulkan>();
//}

#define VERIFYVULKANRESULT_EXTERNAL(VkFunction) { const VkResult ScopedResult = VkFunction; if (ScopedResult != VK_SUCCESS) { GetIVulkanDynamicRHI()->RHIVerifyResult(ScopedResult, #VkFunction, __FILE__, __LINE__); }}
