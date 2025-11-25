#pragma once
#include "id.h"


class Handle
{
public:

	
	Handle(Id::IdType type);
	~Handle();



	uint16_t GetHandle()
	{
		return m_Handle;
	}
private:

	uint16_t m_Handle;
	Id::IdType m_Type;
};