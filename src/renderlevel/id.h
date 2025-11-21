#pragma once

#include <vector>
#include <unordered_map>
#include <stdint.h>



class Id
{
public:
	enum IdType
	{
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

	uint16_t Alloc(Id::IdType type)
	{
		m_Count[type]++;
		m_Handle2Idx[type][m_HandleArrs[type][m_Count[type]]] = m_Count[type];
		return m_HandleArrs[type][m_Count[type]];
	}

	void Free(Id::IdType type,uint16_t handle)
	{
		uint16_t idx = m_Handle2Idx[type][handle];

		uint16_t lasthandle = m_HandleArrs[type][m_Count[type]];
		m_HandleArrs[type][idx] = lasthandle;
		m_HandleArrs[type][m_Count[type]] = handle;

		m_Handle2Idx[type][handle] = m_Count[type];
		m_Handle2Idx[type][lasthandle] = idx;

		m_Count[type]--;

	}
private:

	std::vector<std::vector<uint16_t>> m_HandleArrs;
	std::vector <std::unordered_map<uint16_t, uint16_t>> m_Handle2Idx;
	std::vector<uint32_t> m_Count;

	IdManager()
		: m_Count()
		, m_HandleArrs()
		, m_Handle2Idx()
	{
		m_Count.resize(Id::Count);
		m_HandleArrs.resize(Id::Count);
		m_Handle2Idx.resize(Id::Count);
		for (int i = 0; i < Id::Count; i++)
		{
			m_Count[i] = 0;
			m_HandleArrs[i].resize(512);
			for (size_t j = 0; j < 512; j++)
			{
				m_HandleArrs[i][j] = j;
			}
		}
	}
};