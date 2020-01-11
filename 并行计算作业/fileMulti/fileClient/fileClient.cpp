#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "CBlockingSocket.h"
#include <windows.h>
#include <stdio.h>

using namespace std;


char filename2[100] = { "C:\\Users\\zhushiyuan\\Desktop\\1.txt" };



char serverIP[100] = { "192.168.3.15" };
char serverPORT[100] = { 0 };
int serverPORT2 = 6666;
char conNum[100] = { 0 };
int m = 0;
int i;
HANDLE ghMutex;
fstream fs;
int port = 6666;
DWORD WINAPI clientthread(LPVOID);

//ʹ�û����ź���
DWORD WINAPI clientthread(LPVOID lpParameter) {

	_itoa(serverPORT2, serverPORT, 10);
	cout << "�ͷ���˽��������� " << serverIP << ":" << serverPORT << endl;
	CBlockingSocket BlockingSocket = CBlockingSocket();
	BlockingSocket.Open(serverIP, serverPORT);
	cout << "���ڽ����ļ�... " << endl;
	_itoa(i, conNum, 10);

	BlockingSocket.Send(conNum, 100);

	WaitForSingleObject(
		ghMutex,    
		INFINITE);

	char recvBuf[101] = { 0 };
	int filerecvd = 0;
	
	while (filerecvd != -1)
	{
		filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
		
		fs.write(recvBuf, filerecvd);

	}
	
	return 0;

}

int main(void) {
	HANDLE aThread[3];
	DWORD ThreadID;

	ghMutex = CreateMutex(
		NULL,              
		FALSE,             
		NULL);             

	if (ghMutex == NULL)
	{
		printf("�����ź���ʧ��: %d\n", GetLastError());
		return 1;
	}
	fs.open(filename2, ios::out | ios::binary);

	for (i = 0; i < 3; i++)
	{
		aThread[i] = CreateThread(
			NULL,      
			0,          
			(LPTHREAD_START_ROUTINE)clientthread,
			NULL,      
			0,          
			&ThreadID); 

		if (aThread[i] == NULL)
		{
			printf("�����߳�ʧ��: %d\n", GetLastError());
			return 1;
		}

		Sleep(200);
		serverPORT2++;
	}
	WaitForMultipleObjects(3, aThread, TRUE, INFINITE);
	printf("�ļ��������...");
	Sleep(5000);
	for (i = 0; i < 3; i++)
		CloseHandle(aThread[i]);
	CloseHandle(ghMutex);

}