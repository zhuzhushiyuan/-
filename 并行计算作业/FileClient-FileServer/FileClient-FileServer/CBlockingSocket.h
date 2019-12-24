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
	WSADATA wsaData;                      //�����洢 WSAStartup�������ú󷵻ص�����
	int err = WSAStartup(wVersionRequestd, &wsaData);//WSAStartup�����ϵͳ˵��������Ҫ���Ǹ����ļ����øÿ��ļ��뵱ǰӦ�ó���󶨣��Ӷ��Ϳ��Ե��øð汾�ĸ��ֺ���
	if (err != 0)
	{
		cout << "����Winsocket ʧ��!" << endl;
		this->m_socket = NULL;
		return;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		cout << "�����׽���ʧ��" << endl;
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
		cout << "WSACleanup ʧ��!" << endl;
	}
}

BOOL CBlockingSocket::Open(const char *ip, const char *port)
{
	struct addrinfo *result = NULL;//
	struct addrinfo *ptr = NULL;//

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(port));//htons�ǽ����ͱ����������ֽ�˳��ת��������ֽ�˳�� ���������ڵ�ַ�ռ�洢��ʽ��Ϊ��λ�ֽڴ�����ڴ�ĵ͵�ַ����
	if (connect(m_socket, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		return TRUE;
	}
	else
	{
		cout << "���׽���ʧ��!" << endl;
		return FALSE;
	}
}

BOOL CBlockingSocket::Close()
{
	if (closesocket(m_socket) == 0)
	{
		cout << "���ӹر�" << endl;
		return TRUE;
	}
	else
	{
		cout << "����ʧ��!" << endl;
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
		cout << "����ʧ��!" << endl;
		return FALSE;
	}
}

INT CBlockingSocket::Read(BYTE *s, UINT count)
{
	int recvLen = recv(m_socket, (char *)s, count, 0);
	if (recvLen == SOCKET_ERROR)
	{
		cout << "����ʧ��!" << endl;
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
		printf("���ӹر�\n");
		return -1;
	}
	else
	{
		printf("����ʧ��: %d\n", WSAGetLastError());
		return -1;
	}
}

void CBlockingSocket::Cleanup()
{
	WSACleanup();
}