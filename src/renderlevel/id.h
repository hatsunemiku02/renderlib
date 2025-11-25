#pragma once

#include <vector>
#include <unordered_map>
#include <stdint.h>



class Id
{
public:
	enum IdType
	{
		Material,
		Shader,
		Param,
		Count
	};
	IdType m_Type;
	uint16_t m_Idx;
private:
};

class IdManager
{
public:

	static IdManager& GetInstance()
	{
		static IdManager instance;
		return instance;
	}

	uint16_t Alloc(Id::IdType type);

	void Free(Id::IdType type, uint16_t handle);

	void operator= (const IdManager&) = delete;
	IdManager(const IdManager&) = delete;

private:

	std::vector<std::vector<uint16_t>> m_HandleArrs;
	std::vector <std::unordered_map<uint16_t, uint16_t>> m_Handle2Idx;
	std::vector<uint32_t> m_Count;

	IdManager();
};