#include <WinSock2.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

class CBlockingSocket
{
private:
	SOCKET m_socket;//
public:
	CBlockingSocket();                                   
	virtual		~CBlockingSocket();
	BOOL		Open(const char *ip, const char *port);  
	BOOL		Close();                                
	BOOL		Listen(const char *port);             
	BOOL		Accept();                             
	BOOL		Send(BYTE *s, UINT count);           
	INT			Read(BYTE *s, UINT count);            
	INT         Recv(char *recvbuf, int recvbuflen);
	static void Cleanup();
};


CBlockingSocket::CBlockingSocket()
{
	WORD wVersionRequestd = MAKEWORD(2, 2);
	WSADATA wsaData;                      //用来存储 WSAStartup函数调用后返回的数据
	int err = WSAStartup(wVersionRequestd, &wsaData);//WSAStartup向操作系统说明，我们要用那个库文件，让该库文件与当前应用程序绑定，从而就可以调用该版本的各种函数
	if (err != 0)
	{
		cout << "加载Winsocket 失败!" << endl;
		this->m_socket = NULL;
		return;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		cout << "创建套接字失败" << endl;
		this->m_socket = NULL;
		return;
	}
}

CBlockingSocket::~CBlockingSocket()
{
	if (m_socket != NULL)
	{
		Close();
	}
	if (WSACleanup() == SOCKET_ERROR)
	{
		cout << "WSACleanup 失败!" << endl;
	}
}

BOOL CBlockingSocket::Open(const char *ip, const char *port)
{
	struct addrinfo *result = NULL;//
	struct addrinfo *ptr = NULL;//

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(port));//htons是将整型变量从主机字节顺序转变成网络字节顺序， 就是整数在地址空间存储方式变为高位字节存放在内存的低地址处。
	if (connect(m_socket, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		return TRUE;
	}
	else
	{
		cout << "打开套接字失败!" << endl;
		return FALSE;
	}
}

BOOL CBlockingSocket::Close()
{
	if (closesocket(m_socket) == 0)
	{
		cout << "连接关闭" << endl;
		return TRUE;
	}
	else
	{
		cout << "连接失败!" << endl;
		return FALSE;
	}
}

BOOL CBlockingSocket::Listen(const char *port)
{
	return TRUE;
}

BOOL CBlockingSocket::Accept()
{
	return TRUE;
}

BOOL CBlockingSocket::Send(BYTE *s, UINT count)
{
	int sendLen = send(m_socket, (char *)s, count, 0);
	if (sendLen > 0)
	{
		return TRUE;
	}
	else
	{
		cout << "发送失败!" << endl;
		return FALSE;
	}
}

INT CBlockingSocket::Read(BYTE *s, UINT count)
{
	int recvLen = recv(m_socket, (char *)s, count, 0);
	if (recvLen == SOCKET_ERROR)
	{
		cout << "接收失败!" << endl;
		return -1;
	}
	return recvLen;
}

INT CBlockingSocket::Recv(char *recvbuf, int recvbuflen)
{
	int iResult;
	iResult = recv(m_socket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{

		return iResult;
	}
	else if (iResult == 0)
	{
		printf("连接关闭\n");
		return -1;
	}
	else
	{
		printf("接收失败: %d\n", WSAGetLastError());
		return -1;
	}
}

void CBlockingSocket::Cleanup()
{
	WSACleanup();
}