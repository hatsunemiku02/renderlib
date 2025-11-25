#include "handle.h"

Handle::Handle(Id::IdType type)
:m_Type(type)
{
	m_Handle = IdManager::GetInstance().Alloc(m_Type);
}

Handle::~Handle()
{
	IdManager::GetInstance().Free(m_Type, m_Handle);
}
