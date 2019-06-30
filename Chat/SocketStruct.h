

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

	//������Ϣ���ͻ���
	bool sendMessageToAllClient(SocketStruct &socketclient, char *messages) {
		m.lock();
		cout << "��������" << socketclient.getName() << "����Ϣ��" << messages << "�����пͻ���" << endl;
		for (int i = 0; i < this->size; i++)
		{
			if (socketclient.getName()!= sockets[i].getName()&&socketclient.getSocket()!=NULL)
			{
				cout << "���͸���" << sockets[i].getName() << endl;
				send(sockets[i].getSocket(), socketclient.getName(), 100, 0);
				send(sockets[i].getSocket(), messages, 100, 0);
			}
		}
		m.unlock();
		return true;
	}
	//��ӵ�socket���Ӽ�����ȥ
	void addSocket(const SocketStruct& s) {
		m.lock();
		cout << "��ӵ�������" << endl;
		this->sockets.push_back(s);
		size++;
		cout << "��������������";
		for (int i = 0; i < size; i++)
		{
			cout<< sockets[i].getName() << endl;
		}
		m.unlock();
		
	}

	//�������߳����ڼ����ͻ��˷�������Ϣ���ú���
	void newThreadtoSendMessage(SocketStruct socketClient) {
		cout << "�����������߳�" << endl;
		char username[100];//�����û���
		char password[100];//��������
		try {
			//���ղ���ӡ�ͻ�������
			recv(socketClient.getSocket(), username, 100, 0);
			recv(socketClient.getSocket(), password, 100, 0);
			//��֤����ע���û�
			cout << username << "���������߳�" << endl;
			send(socketClient.getSocket(), "���������ӳɹ�", 100, 0);
			cout << username << "�û�������" << endl;
			m.lock();
			socketClient.setName(username);
			m.unlock();
			this->addSocket(socketClient);

			while (true)
			{
				char messsge[100];
				recv(socketClient.getSocket(), messsge, 100, 0);
				cout << socketClient.getSocket();
				cout << "���������յ�����" << username << "����Ϣ��" << messsge << endl;
				this->sendMessageToAllClient(socketClient, messsge);
				}
			
		}catch(exception e){
				cout << e.what() << endl;
		}
		//�ر�socket
		closesocket(socketClient.getSocket());
		WSACleanup();
		socketClient.setSocket();
		
	}


	void Listen() {

		WSADATA wsdata = { 0 };
		WSAStartup(MAKEWORD(2, 2), &wsdata);   //Winsock����ĳ�ʼ������仰һ��Ҫ����ǰ��
		SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addserver;
		addserver.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
		addserver.sin_family = AF_INET;
		addserver.sin_port = htons(6000);
		auto res = ::bind(socketServer, (SOCKADDR*)&addserver, sizeof(SOCKADDR));

		listen(socketServer, 5);
		printf("������������:\n������...\n");

		SOCKET socketClient;
		SOCKADDR_IN addClient;
		int len = sizeof(SOCKADDR);
		

		while (true) {
			//���������̣�ֱ���пͻ�����������Ϊֹ
			socketClient = accept(socketServer, (SOCKADDR*)&addClient, &len);
			SocketStruct s = SocketStruct(socketClient, "");
			thread *t = new thread(&Server::newThreadtoSendMessage,this, std::ref(s));
			t->detach();
		}

		
	}
};




