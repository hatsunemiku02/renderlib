#pragma once



class VBOVulkan;

class Draw
{
public:
	Draw();
	~Draw();
	void Execute();
private:
	VBOVulkan* m_pVbo;
};