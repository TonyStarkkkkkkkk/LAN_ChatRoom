﻿#pragma once
#ifndef _CLIENTLIST_H_
#define _CLIENTLIST_H_
#include <vector>
#include "CSocket.h"
#include <assert.h>//assert.h常用于防御式编程。断言（Assertions），一个断言通常是一个例程（routines）或者一个宏（marcos）。每个断言通常含有两个参数：一个布尔表示式（a boolean expression）和一个消息（a message）。
class CSocket;
class ClientList
{
public:
	typedef vector<CSocket*>::iterator Iter;//typedef:类型定义   iterator:迭代器  
	void Add(CSocket* socket);
	int Count() const;
	CSocket* operator[](size_t index);//index:指针
	void Remove(CSocket* socket);//移除
	Iter Find(CSocket* socket);//查找
	void Clear();
	void Broadcast(char* str, int len, CSocket* s3);
	void ShowVec(const vector<int>& valList);
	static ClientList* GetInstance()//定义一个全局客户端list列表  GetInstance()实例
	{
		static ClientList instance;
		return &instance;//返回实例地址
	}
	~ClientList();//析构  释放内存
private:
	static CRITICAL_SECTION g_cs;
	static vector<CSocket*> _list;//list列表中是socket静态全局对象
	ClientList();
	ClientList(const ClientList&);
	ClientList& operator=(const ClientList&);
};
#endif
