

#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "CBlockingSocket.h"

using namespace std;

int main(int argc, char **argv)

{
	if (argc != 3)
	{
		cout << "�����ʽ����" << endl;
		cout << "FileClient ����IP 8888" << endl;
	}



		CBlockingSocket BlockingSocket = CBlockingSocket();
		if (BlockingSocket.Open(argv[1], argv[2]))
		{
			cout << " ��Զ�̷���˽��������ڣ�" << argv[1] << ":" << argv[2] << endl;
			cout << "������Ҫ��ȡ���ļ�·��: ";

			BYTE filename[100];
			cin >> filename;

			if (BlockingSocket.Send(filename, 100))
			{
				cout << "  �ͻ��������ļ�: " << filename << endl;

				cout << "  �Ƿ񱣴��ڱ���" << endl;
				//cout << "  2.��ӡ�ڿ��ƿ���" << endl;
				cout << "  ����1�Ǳ��棬2���˳�";

				int put;
				cin >> put;
				cout << " ----------------------------------" << endl;

				if (put == 1)
				{
					char filename2[100] = { 0 };
					cout << " ���뱣���·��:";
					cin >> filename2;

					cout << "  ���ڽ����ļ� " << filename << endl;
					fstream fs;//
					char recvBuf[101] = { 0 };
					fs.open(filename2, ios::out | ios::binary);
					int filerecvd = 0;
					while (filerecvd != -1)
					{
						filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
						//cout << "  ���յ� " << filerecvd << " �ֽ�" << endl;
						fs.write(recvBuf, filerecvd);
					}
				}
				else if (put == 2) {
					return 1;

					/*int recvLen = 0;
					BYTE recvBuf[101] = { 0 };
					int len = 0;
					while ((len = BlockingSocket.Read(recvBuf, 100)) != -1 && len != 0)
					{
						recvLen += len;
						cout << recvBuf;
					}
					cout << endl << " ----------------------------------" << endl;
					cout << " ���յ� " << recvLen << " �ֽ�" << endl;*/
				}
			}
		}
	
	return 0;
}