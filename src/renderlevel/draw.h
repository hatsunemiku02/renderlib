#pragma once

#include <stdint.h>

class VBOVulkan;
class Material;

class Draw
{
public:
	Draw();
	~Draw();
	
	void Execute();

	uint64_t GetSort() const;
private:
	VBOVulkan* m_pVbo;
	Material* m_pMat;
};