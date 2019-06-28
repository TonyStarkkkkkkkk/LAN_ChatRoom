#include "stdafx.h"
#include "CSocket.h"
CSocket::CSocket(SocketEnum::SocketType _socketType) :csocket(INVALID_SOCKET), isConnected(false), buffer(NULL), sendCount(0), recvCount(0), isBlocking(true), socketError(SocketEnum::InvalidSocket), socketType(_socketType) {}
bool CSocket::Connect(const char* ip, int port)
{
	isConnected = true;
	socketError = SocketEnum::Success;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//初始化套接字DLL
	{
		SetSocketError(SocketEnum::WSAStartupError);//错误
		isConnected = false;//失败返回false
	}
	if (isConnected)//如果true
	{
		if ((csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)//
		{
			SetSocketError();
			isConnected = false;//返回失败false
		}
	}
	if (isConnected)//如果true
	{
		memset(&serverAddress, 0, sizeof(sockaddr_in));//memset:内存
		serverAddress.sin_family = AF_INET;
		long lip = inet_addr(ip);
		if (lip == INADDR_NONE)
		{
			SetSocketError(SocketEnum::InvaliAddress);
			isConnected = false;
		}
		else
		{
			if (port < 0)//如果端口号小于0
			{
				SetSocketError(SocketEnum::InvalidPort);
				isConnected = false;//返回失败false
			}
			else
			{
				serverAddress.sin_addr.S_un.S_addr = lip;
				serverAddress.sin_port = htons(port);
				if (connect(csocket, (sockaddr*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
				{
					SetSocketError();
					isConnected = false;//返回失败false
				}
			}
		}
	}
	return isConnected;//返回成功true
}
//设置阻塞模式
bool CSocket::SetBlocking(bool isBlock)
{
	int block = isBlock ? 0 : 1;
	if (ioctlsocket(csocket, FIONBIO, (ULONG*)& block) != 0)
	{
		return false;
	}
	isBlocking = isBlock;
	return true;
}
int CSocket::Send(char* pBuf, int len)
{
	if (!IsSocketValid() || !isConnected)
	{
		return 0;
	}
	if (pBuf == NULL || len < 1)
	{
		return 0;
	}
	sendCount = send(csocket, pBuf, len, 0);
	if (sendCount <= 0)
	{
		cout << GetSocketError() << endl;
	}
	return sendCount;
}
//Receive:收到，接纳;
int CSocket::Receive(int strLen)//接收套接字
{
	recvCount = 0;
	if (!IsSocketValid() || !isConnected)
	{
		return recvCount;
	}
	if (strLen < 1)
	{
		return recvCount;
	}
	if (buffer != NULL)
	{
		delete buffer;
		buffer = NULL;
	}
	buffer = new char[strLen];
	SetSocketError(SocketEnum::Success);
	while (1)
	{
		recvCount = recv(csocket, buffer, strLen, 0);
		if (recvCount > 0)
		{
			buffer[recvCount] = '\0';
			if (IsExit())
			{
				Send(buffer, recvCount);
				delete buffer;
				buffer = NULL;
				recvCount = 0;
				break;
			}
			else
			{
				cout << "buffer" << endl;
				p->Broadcast(buffer, strlen(buffer), this);
				cout << buffer << endl;
			}
		}
	}
	return recvCount;
}
void CSocket::diaoyong(ClientList* Clien2)
{
	p = Clien2;
}
bool CSocket::IsExit()
{
	int len = strlen(buffer);
	int i = 0;
	int size = 4;
	if (len == size)
	{
		char* exit = "EXIT";
		for (i = 0; i < size; i++)
		{
			if (buffer[i] != *(exit + 1) && buffer[i] - 32 != *(exit + i))
			{
				break;
			}
		}
	}
	return i == size;
}
//设置错误信息
void CSocket::SetSocketError(SocketEnum::SocketError error)
{
	socketError = error;
}
void CSocket::SetSocketError(void)//枚举socket返回的错误信息
{
	int nError = WSAGetLastError();
	switch (nError)
	{
	case EXIT_SUCCESS:
		SetSocketError(SocketEnum::Success);
		break;
	case WSAEBADF:
	case WSAENOTCONN:
		SetSocketError(SocketEnum::Notconnected);
		break;
	case WSAEINTR:
		SetSocketError(SocketEnum::Interrupted);
		break;
	case WSAEACCES:
	case WSAEAFNOSUPPORT:
	case WSAEINVAL:
	case WSAEMFILE:
	case WSAENOBUFS:
	case WSAEPROTONOSUPPORT:
		SetSocketError(SocketEnum::InvalidSocket);
		break;
	case WSAECONNREFUSED:
		SetSocketError(SocketEnum::ConnectionRefused);
		break;
	case WSAETIMEDOUT:
		SetSocketError(SocketEnum::Timedout);
		break;
	case WSAEINPROGRESS:
		SetSocketError(SocketEnum::Einprogress);
		break;
	case WSAECONNABORTED:
		SetSocketError(SocketEnum::ConnectionAborted);
		break;
	case WSAEWOULDBLOCK:
		SetSocketError(SocketEnum::Ewouldblock);
		break;
	case WSAENOTSOCK:
		SetSocketError(SocketEnum::InvalidSocket);
		break;
	case WSAECONNRESET:
		SetSocketError(SocketEnum::ConnectionReset);
		break;
	case WSANO_DATA:
		SetSocketError(SocketEnum::InvaliAddress);
		break;
	case WSAEADDRINUSE:
		SetSocketError(SocketEnum::AddressInUse);
		break;
	case WSAEFAULT:
		SetSocketError(SocketEnum::InvalidPointer);
		break;
	default:
		SetSocketError(SocketEnum::UnknownError);
		break;
	}
}
//valid:有效的
bool CSocket::IsSocketValid(void)//有效的套接字
{
	return socketError == SocketEnum::Success;//返回Success信息
}
SocketEnum::SocketError CSocket::GetSocketError()
{
	return socketError;
}
CSocket::~CSocket()
{
	Close();//关闭
}
void CSocket::Close()
{
	if (buffer != NULL)//如果不为空
	{
		delete buffer;//删除储存的信息
		buffer = NULL;//存储段为空
	}
	ShutDown(SocketEnum::Both);
	if (closesocket(csocket) != SocketEnum::Error)
	{
		csocket = INVALID_SOCKET;
	}
	 /* WSACleanup();//清理套接字占用的资源*/
}
//ShutDown:关机      mode:方式
bool CSocket::ShutDown(SocketEnum::ShutdownMode mode)
{
	SocketEnum::SocketError nRetVal = (SocketEnum::SocketError)shutdown(csocket, SocketEnum::Both);
	SetSocketError();
	return(nRetVal == SocketEnum::Success) ? true : false;
}
char* CSocket::GetData()
{
	return buffer;
}
void CSocket::SetSocketHandle(SOCKET socket)
{
	if (socket != SOCKET_ERROR)
	{
		csocket = socket;
		isConnected = true;
		socketError = SocketEnum::Success;
	}
}
bool CSocket::operator==(const CSocket* socket)
{
	return csocket == socket->csocket;
}
