
#include <iostream>

#include <Winsock2.h>
#include <cstdio>
#include"SocketStruct.h"

#include <thread>
#include<string.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
/*
*/


int main()
{
	Server *s = new Server();
	s->Listen();
}