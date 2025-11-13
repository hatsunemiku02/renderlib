#include "VBOVulkan.h"
#include "BufferTransfer.h"
#include "../DeviceVulkan.h"
#include <vector>


//typedef struct VkVertexInputBindingDescription {
//    uint32_t             binding;
//    uint32_t             stride;
//    VkVertexInputRate    inputRate;
//} VkVertexInputBindingDescription;
//
//typedef struct VkVertexInputAttributeDescription {
//    uint32_t    location;
//    uint32_t    binding;
//    VkFormat    format;
//    uint32_t    offset;
//} VkVertexInputAttributeDescription;

std::vector<float> Vertices =
{
	 0.5f, 0.5f, 0.0f, 1.0f,1.0f,
	 -0.5f, 0.5f, 0.0f,-0.0f,1.0f,
	 0.f,-0.5f, 0.0f, 0.5f,-1.0f,
};

//VkVertexInputBindingDescription bindingVtx = {
//	0,
//	5 * sizeof(float),
//	VK_VERTEX_INPUT_RATE_VERTEX,
//};
//
//VkVertexInputAttributeDescription descVtx = {
//	0,0,VK_FORMAT_R32G32B32_SFLOAT,0
//};
//
//VkVertexInputBindingDescription bindingUV = {
//	0,
//	5 * sizeof(float),
//	VK_VERTEX_INPUT_RATE_VERTEX,
//};


std::shared_ptr<VertexInputDescription> desc= std::make_shared<VertexInputDescription>();



//std::vector<VkVertexInputBindingDescription> attributeDescriptions = {
//{ 0, 5 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX},
//{ 0, 5 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX},
//};
//
//
//std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
//{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
//{ 0, 1, VK_FORMAT_R32G32_SFLOAT, sizeof(float)},
//};

VBOVulkan::VBOVulkan()
{
}

VBOVulkan::~VBOVulkan()
{
}

void VBOVulkan::DefaultCreate(const DeviceVulkan& device)
{
	CreateVBO(device, Vertices.data(), sizeof(float) * 3 * 5);
	desc->bindings = {
		{ 0, 5 * sizeof(float), VK_VERTEX_INPUT_RATE_VERTEX },
	};

	desc->attributes = {
			{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
			{ 1, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(float)},
	};
	SetVBODesc(desc);
}

void VBOVulkan::CreateVBO(const DeviceVulkan& device, void* data, uint32_t size)
{
	m_StagingxBuffer.Allocate(device, data, size, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	m_VertexBuffer.Allocate(device,size, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT|VK_BUFFER_USAGE_TRANSFER_DST_BIT);

	BufferTransfer::GetInstance().AddTransferCmd(&m_StagingxBuffer, &m_VertexBuffer,size, 0, 0);
}

void VBOVulkan::SetVBODesc(const std::shared_ptr<VertexInputDescription>& desc)
{
	m_pVertexDesc = desc;
}
