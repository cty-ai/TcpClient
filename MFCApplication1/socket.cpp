#include "stdafx.h"
#include "socket.h"
#include <WS2tcpip.h>


eqkSocket::eqkSocket()
{
	mConnTimeout = 0;
	mRecvTimeout = 10 * 1000;
}

eqkSocket::eqkSocket(int connTimeout, int recvTimeout)
{
	mConnTimeout = connTimeout;
	mRecvTimeout = recvTimeout;
}

eqkSocket::~eqkSocket()
{
}

bool eqkSocket::initWinSock()
{
	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int intEr = WSAStartup(verision, &lpData);
	if (intEr != 0)
	{
		printf("WinSock init failed！\n");
		return false;
	}
	return true;
}

bool eqkSocket::createSocket()
{
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		printf("scoket create failed！\r\n");
		return false;
	}
	int reuse = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	int ms = mRecvTimeout;
	setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&ms, sizeof(int));

	return true;
}

bool eqkSocket::connectSocket(std::string ip, const unsigned short port)
{
	bool res = true;
	SOCKADDR_IN hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);
	struct in_addr s;
	inet_pton(AF_INET, ip.c_str(), (void*)&s);
	hostAddr.sin_addr = s;

	int err;

	if (mConnTimeout <= 0){/* default connect timeout by kernel */
		err = connect(mSocket, (sockaddr*)&hostAddr, sizeof(sockaddr));
		if (err == INVALID_SOCKET)
		{
			printf("connect failed！\r\n");
			return false;
		}
	}
	else{
		unsigned long iMode = 1;
		err = ioctlsocket(mSocket, FIONBIO, &iMode);
		if (err != NO_ERROR){
			printf("bio set failed[%d].\r\n", GetLastError());
			return false;
		}

		struct timeval timeout;
		fd_set r;
		FD_ZERO(&r);
		FD_SET(mSocket, &r);
		timeout.tv_sec = 0;
		timeout.tv_usec = mConnTimeout * 1000L;

		err = connect(mSocket, (sockaddr*)&hostAddr, sizeof(sockaddr));
		if (err < 0)
		{
			err = select(mSocket + 1, NULL, &r, NULL, &timeout);
			if (err <= 0){
				printf("connect timeout[%d ms]\r\n", mConnTimeout);
				res = false;
			}
		}

		iMode = 0;
		err = ioctlsocket(mSocket, FIONBIO, &iMode);
		if (err != NO_ERROR){
			printf("non-bio set failed. \r\n");
			res = false;
		}
	}

	return res;
}

bool eqkSocket::connectCheck(std::string ip, const unsigned short port)
{
	mSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocket == INVALID_SOCKET)
	{
		printf("scoket create failed！\r\n");
		return false;
	}
	int reuse = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));

	bool res = false;
	SOCKADDR_IN hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);
	struct in_addr s;
	inet_pton(AF_INET, ip.c_str(), (void*)&s);
	hostAddr.sin_addr = s;

	int err;
	{
		unsigned long iMode = 1;
		err = ioctlsocket(mSocket, FIONBIO, &iMode);
		if (err != NO_ERROR){
			printf("bio set failed. \r\n");
			closesocket(mSocket);
			return false;
		}

		struct timeval timeout;
		fd_set r, w;
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_SET(mSocket, &r);
		FD_SET(mSocket, &w);
		timeout.tv_sec = 0;
		timeout.tv_usec = mConnTimeout * 1000L;

		err = connect(mSocket, (sockaddr*)&hostAddr, sizeof(sockaddr));
		if (err < 0)
		{
			err = select(mSocket + 1, &r, &w, NULL, &timeout);
			if (err == 1 && FD_ISSET(mSocket, &w)){
				res = true;
			}
		}
		else {
			res = true;
		}
	}

	closesocket(mSocket);
	return res;
}

bool eqkSocket::sendData(std::string &data)
{
	int ret;
	int sent = 0;
	int total = data.length();
	do{
		ret = send(mSocket, data.c_str() + sent, total - sent, 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send data error[%d]！\r\n", GetLastError());
			return false;
		}
		else{
			sent += ret;
		}
	} while (sent < total);

	return true;
}

bool eqkSocket::sendDataExternal(SOCKET &socket, std::string &data)
{
	int ret;
	int sent = 0;
	int total = data.length();
	do{
		ret = send(socket, data.c_str() + sent, total - sent, 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send data error[%d]！\r\n", GetLastError());
			return false;
		}
		else{
			sent += ret;
		}
	} while (sent < total);

	return true;
}

bool eqkSocket::sendData2(const char* data, size_t dataLen)
{
	int sent = 0;
	do{
		int ret = send(mSocket, data + sent, dataLen - sent, 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send data error[%d]！\r\n", GetLastError());
			return false;
		}
		else{
			sent += ret;
		}
	} while (sent < dataLen);

	return true;
}

/* 发送完数据后关闭发送 */
bool eqkSocket::sendDataAndShutdown(std::string &data)
{
	int ret;
	int sent = 0;
	int total = data.length();
	do{
		ret = send(mSocket, data.c_str() + sent, total - sent, 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send data error[%d]！\r\n", GetLastError());
			return false;
		}
		else{
			sent += ret;
		}
	} while (sent < total);

	ret = shutdown(mSocket, SD_SEND);
	if (ret == SOCKET_ERROR){
		printf("shutdown error[%d]！\r\n", GetLastError());
		return false;
	}

	return true;
}

bool eqkSocket::receiveData(std::string &data)
{
	char *buf;
	buf = (char *)malloc(MSG_BUFFER_LEN);
	if (!buf){
		printf("no memory\r\n");
		return false;
	}

	int buflen;
	do{
		memset(buf, 0, MSG_BUFFER_LEN);
		buflen = recv(mSocket, buf, MSG_BUFFER_LEN - 1, 0);
		if (buflen > 0){
			data += buf;
			//printf("recv loop \r\n");
		}
		else if (buflen == 0){
			//printf("finally complete receving [%d] bytes\r\n", data.GetLength());
			break;
		}
		else
		{
			printf("receive data failed！\r\n");
			free(buf);
			return false;
		}
	} while (buflen > 0);

	free(buf);

	return true;
}

bool eqkSocket::receiveData(SOCKET sk, std::string &data)
{
	char *buf;
	buf = (char *)malloc(MSG_BUFFER_LEN);
	if (!buf){
		printf("no memory\r\n");
		return false;
	}

	int buflen;
	data.clear();
	do{
		memset(buf, 0, MSG_BUFFER_LEN);
		buflen = recv(sk, buf, MSG_BUFFER_LEN - 1, 0);
		if (buflen > 0){
			data.append(buf);
		}
		else if (buflen == 0){
			break;
		}
		else
		{
			printf("receive data failed！\r\n");
			free(buf);
			return false;
		}
	} while (buflen > 0);

	free(buf);

	return true;
}

bool eqkSocket::closeSocket()
{
	int ret = closesocket(mSocket);
	if (ret == SOCKET_ERROR)
	{
		printf("ERROR: closesocket unsuccessful\r\n");
		return false;
	}
	return true;
}

bool eqkSocket::closeSocket(SOCKET sk)
{
	int ret = closesocket(sk);
	if (ret == SOCKET_ERROR)
	{
		printf("ERROR: closesocket unsuccessful\r\n");
		return false;
	}
	return true;
}

eqkSocket::SocketErr eqkSocket::ccsrcSocket(std::string hostIp, unsigned short port, std::string& reqStr, std::string& respStr)
{
	if (!createSocket()) {
		return SOCKET_CREAT_ERR;
	}
	else if (!connectSocket(hostIp, port)) {
		closeSocket();
		return SOCKET_CONNECT_ERR;
	}
	else if (!sendData(reqStr)) {
		closeSocket();
		return SOCKET_SEND_ERR;
	}
	else if (!receiveData(respStr)) {
		closeSocket();
		return SOCKET_RECV_ERR;
	}
	else {
		closeSocket();
	}

	return SOCKET_NO_ERR;
}

eqkSocket::SocketErr eqkSocket::ccsrcSocketWithHead(std::string hostIp, unsigned short port, std::string& reqStr, std::string& respStr, char *head, int headLen)
{
	if (!createSocket()) {
		return SOCKET_CREAT_ERR;
	}
	else if (!connectSocket(hostIp, port)) {
		closeSocket();
		return SOCKET_CONNECT_ERR;
	}
	else if (!sendData(reqStr)) {
		closeSocket();
		return SOCKET_SEND_ERR;
	}
	else if (!receiveData(respStr)) {
		closeSocket();
		return SOCKET_RECV_ERR;
	}
	else {
		closeSocket();
	}

	return SOCKET_NO_ERR;
}


std::string eqkSocket::getErrStr(SocketErr errCode)
{
	switch (errCode){
	case SOCKET_CREAT_ERR:
		return "SOCKET_CREAT_ERR";
	case SOCKET_CONNECT_ERR:
		return "SOCKET_CONNECT_ERR";
	case SOCKET_SEND_ERR:
		return "SOCKET_SEND_ERR";
	case SOCKET_RECV_ERR:
		return "SOCKET_RECV_ERR";
	default:
		return "UNKNOWN_ERR";
	}
}

