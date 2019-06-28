
#include "ClientList.h"
typedef vector<CSocket*>::iterator Iter;
ClientList::ClientList()
{
	InitializeCriticalSection(&g_cs);//初始化g_cs的成员
}
ClientList::~ClientList()
{
	DeleteCriticalSection(&g_cs);//删除关键段
}
void ClientList::Add(CSocket* socket)
{
	if (socket != NULL)
	{
		EnterCriticalSection(&g_cs);//进入关键段
		_list.push_back(socket);
		LeaveCriticalSection(&g_cs);//退出关键段
	}
}
int ClientList::Count() const//计数socket连接数量
{
	return _list.size();
}
CSocket* ClientList::operator[](size_t index)//index:指针
{
	assert(index >= 0 && index < _list.size());//assert:断言
	return _list[index];
}
void ClientList::Remove(CSocket* socket)
{
	Iter iter = Find(socket);
	EnterCriticalSection(&g_cs);//进入关键段
	if (iter != _list.end())
	{
		delete* iter;
		_list.erase(iter);
	}
	LeaveCriticalSection(&g_cs);//退出关键段
}
Iter ClientList::Find(CSocket* socket)
{
	EnterCriticalSection(&g_cs);//进入关键段
	Iter iter = _list.begin();
	while (iter != _list.end())
	{
		if (*iter == socket)
		{
			return iter;
		}
		iter++;
	}
	LeaveCriticalSection(&g_cs);//退出关键段
	return iter;
}
void ClientList::Clear()
{
	EnterCriticalSection(&g_cs);
	for (int i = _list.size() - 1; i > +0; i--)
	{
		delete _list[i];
	}
	_list.clear();
	LeaveCriticalSection(&g_cs);
	LeaveCriticalSection(&g_cs);
}
void ClientList::Broadcast(char* str, int len, CSocket* s3)//向客户端广播数据
{
	for (int i = 0; i < _list.size(); i++)
	{
		if (_list[i] != s3)
		{
			_list[i]->Send(str, len);
		}
	}
}
void ClientList::ShowVec(const vector<int>& valList)
{
}
CRITICAL_SECTION ClientList::g_cs;
vector<CSocket*>ClientList::_list;