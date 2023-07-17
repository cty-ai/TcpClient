#pragma once
#include <string>
#define MSG_BUFFER_LEN	128 * 1024//10K

class eqkSocket
{
public:
	eqkSocket();
	eqkSocket(int connTimeout, int recvTimeout);
	~eqkSocket();

	enum SocketErr {
		SOCKET_NO_ERR = 0,
		SOCKET_CREAT_ERR,
		SOCKET_CONNECT_ERR,
		SOCKET_SEND_ERR,
		SOCKET_RECV_ERR
	};
	bool initWinSock();
	bool createSocket();
	bool connectSocket(std::string ip, const unsigned short port);
	bool connectCheck(std::string ip, const unsigned short port);
	bool sendData(std::string &data);
	bool sendDataExternal(SOCKET &socket, std::string &data);
	bool sendData2(const char* data, size_t dataLen);
	bool sendDataAndShutdown(std::string &data);
	bool receiveData(std::string &data);
	bool receiveData(SOCKET sk, std::string &data);
	bool closeSocket();
	bool closeSocket(SOCKET sk);
	SocketErr ccsrcSocket(std::string hostIp, unsigned short port, std::string& reqStr, std::string& respStr);
	SocketErr ccsrcSocketWithHead(std::string hostIp, unsigned short port, std::string& reqStr, std::string& respStr, char *head, int headLen);
	std::string getErrStr(SocketErr errCode);

private:
	SOCKET mSocket;
	int mConnTimeout;
	int mRecvTimeout;
};

