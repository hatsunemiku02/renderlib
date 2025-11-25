#include "draw.h"
#include "material.h"
#include "renderlibvulkan/resource/VBOVulkan.h"

Draw::Draw()
	:m_pMat(nullptr)
	,m_pVbo(nullptr)
{
}

Draw::~Draw()
{
}

void Draw::Execute()
{
}

uint64_t Draw::GetSort() const
{
	if (m_pMat)
	{
		return m_pMat->GetHandle();
	}
	return 0;
}
