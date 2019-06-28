﻿#pragma once
#ifndef __CSOCKET_H__
#define __CSOCKET_H__
#include <windows.h>
#include "SocketEnum.h"
#include <iostream>
using namespace std;
#include "ClientList.h"
class ClientList;
class CSocket
{
public:
	CSocket(SocketEnum::SocketType _socketType = SocketEnum::Tcp);
	~CSocket();
	bool Connect(const char* ip, int port);//链接    port:端口
	int Send(char* pBuf, int len);//发送
	int Receive(int strLen);//接收
	bool SetBlocking(bool isBlockinng);//设置阻塞模式
	bool ShutDown(SocketEnum::ShutdownMode mode);
	char* GetData();//获取接收数据
	SocketEnum::SocketError GetSocketError();
	void SetSocketHandle(SOCKET socket);//设置套接字句柄
	void Close();//关闭
	bool operator==(const CSocket* socket);
	bool IsExit();
	void diaoyong(ClientList* Clien2);
	ClientList* p;
private:
	void SetSocketError(SocketEnum::SocketError error);
	void SetSocketError(void);
	bool IsSocketValid(void);
	SOCKET csocket;
	bool isConnected;//链接状态
	struct sockaddr_in serverAddress;
	char* buffer;//存接收数据
	int sendCount;//发送数据长度
	int recvCount;//接收数据长度
	bool isBlocking;//是否是阻塞模式
	SocketEnum::SocketError socketError;
	SocketEnum::SocketType socketType;
	WSADATA wsa;
	//ClientList* p;
};
#endif
