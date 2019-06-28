// Server练习版.cpp : 定义控制台应用程序的入口点。
//
#include <Windows.h>
#include <process.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#include <vector>
#include "SServer.h"
#include "CSocket.h"
#include "ClientList.h"
const int BUF_LEN = 1024;
void recv(PVOID pt)//定义一个接收套接字实例的函数
{
	CSocket* csocket = (CSocket*)pt;//定义CSocket指针类型（套接字）
	if (csocket != NULL)
	{
		int count = csocket->Receive(BUF_LEN);
		if (count == 0)
		{
			ClientList* list = ClientList::GetInstance();
			list->Remove(csocket);
			cout << "一个用户下线，现在的在线人数为:" << list->Count() << endl;
			_endthread();//用户下线，终止接收数据线程
		}
	}
}
void sends(PVOID pt)
{
	ClientList* list = (ClientList*)pt;
	while (1)
	{
		char* buf = new char[BUF_LEN];
		cin >> buf;
		int bufSize = 0;
		while (buf[bufSize++] != '\0');
		for (int i = list->Count() - 1; i >= 0; i--)
		{
			(*list)[i]->Send(buf, bufSize);
		}
		delete buf;
	}
}
int main(int argc, char* argv[])
{
	SServer server;
	bool isStart = server.Start(1986);
	if (isStart)
	{
		cout << "server start success..." << endl;
	}
	else
	{
		cout << "server start error" << endl;
	}
	ClientList* list = ClientList::GetInstance();
	_beginthread(sends, 0, list);//启动一个线程广播数据
	while (1)
	{
		CSocket* csocket = server.Accept();
		csocket->diaoyong(list);
		char* nihao = "holle~";
		csocket->Send(nihao, sizeof(&nihao) + 1);
		list->Add(csocket);
		char* p = "新上线一个用户";
		cout << "新上线一个用户，现在在线人数为:" << list->Count() << endl;
		list->Broadcast(p, strlen(p), csocket);
		_beginthread(recv, 0, csocket);//启动一个接收数据的线程
	}
	getchar();
	return 0;
}
