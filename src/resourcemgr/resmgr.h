#pragma once

class ResMgr
{
public:

	const ResMgr& GetInstance()
	{
		return s_Instance;
	}
	
	~ResMgr() {};

	ResMgr(const ResMgr& rmgr) = delete;
	void operator= (const ResMgr& rmgr) = delete;
private:
	ResMgr() {};
	static ResMgr s_Instance;

};