#pragma once

#include "BufferVulkan.h"

class DeviceVulkan;



class IBOVulkan
{
public:
	IBOVulkan();
	~IBOVulkan();

	void CreateIBO(const DeviceVulkan& device, void* data, uint32_t size);
private:
	BufferVulkan m_IndexBuffer;


};