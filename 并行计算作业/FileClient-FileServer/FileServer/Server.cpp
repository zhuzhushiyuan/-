
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include "CBlockingSocket.h"


using namespace std;





DWORD WINAPI serverthread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	CBlockingSocket ClientSocket = *cs;   //用局部变量保存线程传进来的地址传递的参数，防止主线程中socket被改写。

	BYTE filename[200] = { 0 };//filename：　　要打开的文件名
	if (ClientSocket.Read(filename, 200) != -1)//
	{
		cout << "  来自客户端的文件请求: " << filename << endl;
		ifstream infile((char *)filename, ios::in | ios::binary);//

		infile.seekg(0, ios::end);//ios::end：　　文件结尾
		int fileLength = infile.tellg();
		infile.seekg(0, ios::beg);//ios::beg，文件开头，seekg(0,ios::end);将指针移到文件尾
		bool flag = true;
		if (!infile)
		{
			cout << "文件打开失败" << endl;
		}
		else
		{
			BYTE file[201] = { 0 };//
			while (fileLength >= 200)//
			{
				infile.read((char *)file, 200);
				file[200] = '\0';
				flag = ClientSocket.Send(file, 200);//
				fileLength -= 200;
			}
			infile.read((char *)file, fileLength);//
			file[fileLength] = '\0';
			flag = ClientSocket.Send(file, fileLength);
			if (flag)
			{
				cout << " 将文件发送回客户端: " << filename << endl;
			}
		}
		
	}

	ClientSocket.Close();
	return 0;
}

int main(int argc, char *argv[])
{


	if (argc != 2)
	{
		cout << "FileServer 8888" << endl;//
		return 1;
	}


	CBlockingSocket ListenSocket;
	CBlockingSocket::Initialize();//初始化套接字
	ListenSocket.HintsAndResult(NULL, argv[1]);//
	ListenSocket.Open();//
	ListenSocket.Bind();//
	ListenSocket.Listen(argv[1]);
	cout << "FileServer 正在监听端口:" << argv[1] << endl;
	while (1)//
	{
		cout << "连接中…" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		CBlockingSocket ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		cout << " 接收连接" << inet_ntoa(their_addrin.sin_addr) << endl;

		HANDLE hThread;

		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverthread, &ClientSocket, 0, NULL);

	}




	ListenSocket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}