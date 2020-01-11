
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include "CBlockingSocket.h"


using namespace std;

BYTE filename[100] = { "C:\\Users\\zhushiyuan\\Desktop\\zhui.txt" };
char serverPORT[100] = { 0 };
int i;
int m;
int num = 0;
char conNum[100] = { 0 };
HANDLE ghMutex;
int fileLength;


DWORD WINAPI serverthread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	CBlockingSocket ClientSocket = *cs;
	cout << "进入发的线程..." << endl;
	cout << " 来自客户端的文件请求: " << filename << endl;
	ClientSocket.Read(conNum, 100);
	
	m = atoi(conNum);
	

	ifstream infile((char *)filename, ios::in | ios::binary);
	infile.seekg(0, ios::end);
	fileLength = infile.tellg();
	fileLength = fileLength / 3;
	
  
	num = m;
	infile.seekg(0 + num * fileLength, ios::beg);
	
	num++;

	bool flag = true;

	if (!infile)
	{
		cout << "打开文件失败!" << endl;
	}
	else
	{
		BYTE file[101] = { 0 };
		while (fileLength >= 100)
		{
			infile.read((char *)file, 100);
			file[100] = '\0';
			flag = ClientSocket.Send(file, 100);
			fileLength -= 100;
		}
		infile.read((char *)file, fileLength);
		file[fileLength] = '\0';
		flag = ClientSocket.Send(file, fileLength);
	}
	if (flag)
	{
		cout << "  向客户端发送文件: " << filename << endl;
	}
	
	ClientSocket.Close();
	return 0;
}

DWORD WINAPI cserverthread(LPVOID lpParameter) {
	CBlockingSocket ListenSocket;
	CBlockingSocket::Initialize();
	ListenSocket.HintsAndResult(NULL, serverPORT);
	ListenSocket.Open();
	ListenSocket.Bind();
	ListenSocket.Listen(serverPORT);
	cout << "服务端正在监听端口:" << serverPORT << endl;

	while (1)
	{
		cout << "等待连接…" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		CBlockingSocket ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		cout << "接受连接 " << inet_ntoa(their_addrin.sin_addr) << endl;

		HANDLE hThread;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverthread, &ClientSocket, 0, NULL);

	}




	ListenSocket.Close();
	CBlockingSocket::Cleanup();
}

int main(int argc, char *argv[])
{
	HANDLE aThread;
	DWORD ThreadID;

	cout << "请输入端口号（在默认端口号6666、6667和6668中选择一个）：" << endl;
	cin >> serverPORT;
	
	aThread = CreateThread(
		NULL,      
		0,          
		(LPTHREAD_START_ROUTINE)cserverthread,
		NULL,       
		0,          
		&ThreadID); 

	if (aThread == NULL)
	{
		printf("创建线程失败: %d\n", GetLastError());
		return 1;
	}


	WaitForMultipleObjects(1, &aThread, TRUE, INFINITE);
	printf("线程结束");

	return 0;
}
