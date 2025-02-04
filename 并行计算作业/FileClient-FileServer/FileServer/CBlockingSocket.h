
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class CBlockingSocket
{
private:
	SOCKET m_socket;
	struct addrinfo *m_result;
public:
	CBlockingSocket(void);
	CBlockingSocket(SOCKET socket);
	//virtual		~CBlockingSocket(void);
	BOOL		Open();  
	BOOL		Close();  
	BOOL		Bind();  
	BOOL		Listen(const char *port);  
	BOOL        Connect();
	//BOOL		Accept();  
	CBlockingSocket Accept(struct sockaddr &their_addr);
	BOOL		Send(BYTE *s, UINT count);  
	INT			Read(BYTE *s, UINT count);  
	int HintsAndResult(const char *ip, const char *port);
	static int Initialize();
	static void Cleanup();
};


CBlockingSocket::CBlockingSocket(void)//
{
	m_socket = INVALID_SOCKET;
	m_result = NULL;
}
CBlockingSocket::CBlockingSocket(SOCKET socket)
{
	m_socket = socket;
	m_result = NULL;
}
//CBlockingSocket::~CBlockingSocket(void)
/*{
	if (m_result)
		freeaddrinfo(m_result);
}*/

BOOL CBlockingSocket::Open()//
{
	m_socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);//

	if (m_socket == INVALID_SOCKET) {
		cout << "打开失败!" << endl;
		return FALSE;
	}

	return TRUE;
}

BOOL CBlockingSocket::Close()
{
	closesocket(m_socket);
	return TRUE;
}

BOOL CBlockingSocket::Bind()
{
	int iResult;

	iResult = bind(m_socket, m_result->ai_addr, (int)m_result->ai_addrlen);//
	if (iResult == SOCKET_ERROR) {
		cout << "bind 失败!" << endl;
		return FALSE;
	}

	return TRUE;
}
BOOL CBlockingSocket::Listen(const char *port)//
{

	if (listen(m_socket, 2) == SOCKET_ERROR) {
		cout << "listen 失败!" << endl;
		return FALSE;
	}

	return TRUE;


}
BOOL CBlockingSocket::Connect()//
{
	int iResult;


	iResult = connect(m_socket, m_result->ai_addr, (int)m_result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("无法连接到服务端!\n");
		return FALSE;
	}

	return TRUE;
}

CBlockingSocket CBlockingSocket::Accept(struct sockaddr &their_addr)
{
	int addr_size = sizeof(their_addr);
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;


	ClientSocket = accept(m_socket, &their_addr, &addr_size);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "接收失败!" << endl;
		return INVALID_SOCKET;
	}

	return CBlockingSocket(ClientSocket);
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
	if (recvLen == SOCKET_ERROR || recvLen == 0)
	{
		cout << "接收失败!" << endl;
		return -1;
	}
	return recvLen;
}
int CBlockingSocket::Initialize()
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup 失败: %d\n", iResult);
		return 1;
	}

	return 0;
}
void CBlockingSocket::Cleanup()
{
	WSACleanup();
}

int CBlockingSocket::HintsAndResult(const char *ip, const char *port)//
{
	int iResult;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (ip == NULL)
		hints.ai_flags = AI_PASSIVE;


	iResult = getaddrinfo(ip, port, &hints, &m_result);
	if (iResult != 0) {
		printf("获取地址信息失败: %d\n", iResult);
		return 1;
	}

	return 0;
}