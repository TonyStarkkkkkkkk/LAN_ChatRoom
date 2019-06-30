

#include <Winsock2.h>
#include <cstdio>
#include <iostream>
#include<vector>
#include<thread>
#include <mutex>

#pragma comment(lib,"ws2_32.lib")
using namespace std;
mutex m;
class SocketStruct {
private:
	SOCKET socket;
	char name[100];
public:
	SocketStruct() {};
	SocketStruct(SOCKET &socketclient,const char *nameclient) {
		this->socket = socketclient;
		strcpy_s(this->name, 100, nameclient);
	}
	SOCKET &getSocket() {
		return socket;
	}
	char* getName() {
		return name;
	}
	void setName(char *chars) {
		strcpy_s(this->name, 100, chars);
	}
	void setSocket() {
		socket = NULL;
	}
};
class Server {
private:
	vector<SocketStruct> sockets;
	int size = 0;
public:
	Server() {

	};

	//发送消息给客户端
	bool sendMessageToAllClient(SocketStruct &socketclient, char *messages) {
		m.lock();
		cout << "发送来自" << socketclient.getName() << "的消息：" << messages << "给所有客户端" << endl;
		for (int i = 0; i < this->size; i++)
		{
			if (socketclient.getName()!= sockets[i].getName()&&socketclient.getSocket()!=NULL)
			{
				cout << "发送给：" << sockets[i].getName() << endl;
				send(sockets[i].getSocket(), socketclient.getName(), 100, 0);
				send(sockets[i].getSocket(), messages, 100, 0);
			}
		}
		m.unlock();
		return true;
	}
	//添加到socket连接集合中去
	void addSocket(const SocketStruct& s) {
		m.lock();
		cout << "添加到集合中" << endl;
		this->sockets.push_back(s);
		size++;
		cout << "保留的连接名：";
		for (int i = 0; i < size; i++)
		{
			cout<< sockets[i].getName() << endl;
		}
		m.unlock();
		
	}

	//创建新线程用于监听客户端发来的消息所用函数
	void newThreadtoSendMessage(SocketStruct socketClient) {
		cout << "创建新连接线程" << endl;
		char username[100];//接收用户名
		char password[100];//接收密码
		try {
			//接收并打印客户端数据
			recv(socketClient.getSocket(), username, 100, 0);
			recv(socketClient.getSocket(), password, 100, 0);
			//验证或者注册用户
			cout << username << "启动监听线程" << endl;
			send(socketClient.getSocket(), "服务器连接成功", 100, 0);
			cout << username << "用户已连接" << endl;
			m.lock();
			socketClient.setName(username);
			m.unlock();
			this->addSocket(socketClient);

			while (true)
			{
				char messsge[100];
				recv(socketClient.getSocket(), messsge, 100, 0);
				cout << socketClient.getSocket();
				cout << "服务器接收到来自" << username << "的消息：" << messsge << endl;
				this->sendMessageToAllClient(socketClient, messsge);
				}
			
		}catch(exception e){
				cout << e.what() << endl;
		}
		//关闭socket
		closesocket(socketClient.getSocket());
		WSACleanup();
		socketClient.setSocket();
		
	}


	void Listen() {

		WSADATA wsdata = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsdata);   //Winsock服务的初始化，这句话一定要放在前面
		SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addserver;
		addserver.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示任何IP
		addserver.sin_family = AF_INET;
		addserver.sin_port = htons(6000);
		auto res = ::bind(socketServer, (SOCKADDR*)&addserver, sizeof(SOCKADDR));

		listen(socketServer, 5);
		printf("服务器已启动:\n监听中...\n");

		SOCKET socketClient;
		SOCKADDR_IN addClient;
		int len = sizeof(SOCKADDR);
		

		while (true) {
			//会阻塞进程，直到有客户端连接上来为止
			socketClient = accept(socketServer, (SOCKADDR*)&addClient, &len);
			SocketStruct s = SocketStruct(socketClient, "");
			thread *t = new thread(&Server::newThreadtoSendMessage,this, std::ref(s));
			t->detach();
		}

		
	}
};




