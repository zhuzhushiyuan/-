

#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include "CBlockingSocket.h"

using namespace std;

int main(int argc, char **argv)

{
	if (argc != 3)
	{
		cout << "输入格式错误" << endl;
		cout << "FileClient 本机IP 8888" << endl;
	}



		CBlockingSocket BlockingSocket = CBlockingSocket();
		if (BlockingSocket.Open(argv[1], argv[2]))
		{
			cout << " 与远程服务端建立连接在：" << argv[1] << ":" << argv[2] << endl;
			cout << "输入想要读取的文件路径: ";

			BYTE filename[100];
			cin >> filename;

			if (BlockingSocket.Send(filename, 100))
			{
				cout << "  客户端请求文件: " << filename << endl;

				cout << "  是否保存在本地" << endl;
				//cout << "  2.打印在控制框里" << endl;
				cout << "  输入1是保存，2是退出";

				int put;
				cin >> put;
				cout << " ----------------------------------" << endl;

				if (put == 1)
				{
					char filename2[100] = { 0 };
					cout << " 输入保存的路径:";
					cin >> filename2;

					cout << "  正在接收文件 " << filename << endl;
					fstream fs;//
					char recvBuf[101] = { 0 };
					fs.open(filename2, ios::out | ios::binary);
					int filerecvd = 0;
					while (filerecvd != -1)
					{
						filerecvd = BlockingSocket.Recv(recvBuf, sizeof(recvBuf));
						//cout << "  已收到 " << filerecvd << " 字节" << endl;
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
					cout << " 已收到 " << recvLen << " 字节" << endl;*/
				}
			}
		}
	
	return 0;
}