
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <process.h>
#include "CBlockingSocket.h"


using namespace std;





DWORD WINAPI serverthread(LPVOID lpParameter)
{
	CBlockingSocket *cs = (CBlockingSocket *)lpParameter;
	CBlockingSocket ClientSocket = *cs;   //�þֲ����������̴߳������ĵ�ַ���ݵĲ�������ֹ���߳���socket����д��

	BYTE filename[200] = { 0 };//filename������Ҫ�򿪵��ļ���
	if (ClientSocket.Read(filename, 200) != -1)//
	{
		cout << "  ���Կͻ��˵��ļ�����: " << filename << endl;
		ifstream infile((char *)filename, ios::in | ios::binary);//

		infile.seekg(0, ios::end);//ios::end�������ļ���β
		int fileLength = infile.tellg();
		infile.seekg(0, ios::beg);//ios::beg���ļ���ͷ��seekg(0,ios::end);��ָ���Ƶ��ļ�β
		bool flag = true;
		if (!infile)
		{
			cout << "�ļ���ʧ��" << endl;
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
				cout << " ���ļ����ͻؿͻ���: " << filename << endl;
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
	CBlockingSocket::Initialize();//��ʼ���׽���
	ListenSocket.HintsAndResult(NULL, argv[1]);//
	ListenSocket.Open();//
	ListenSocket.Bind();//
	ListenSocket.Listen(argv[1]);
	cout << "FileServer ���ڼ����˿�:" << argv[1] << endl;
	while (1)//
	{
		cout << "�����С�" << endl;

		struct sockaddr their_addr;
		struct sockaddr_in their_addrin;
		CBlockingSocket ClientSocket = ListenSocket.Accept(their_addr);
		memcpy(&their_addrin, &their_addr, sizeof(their_addr));
		cout << " ��������" << inet_ntoa(their_addrin.sin_addr) << endl;

		HANDLE hThread;

		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)serverthread, &ClientSocket, 0, NULL);

	}




	ListenSocket.Close();
	CBlockingSocket::Cleanup();
	return 0;
}