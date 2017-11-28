#include "ServerSocket.hpp"
#include "SendSocket.hpp"
#include <WinSock2.h>

namespace tcpframework {

	ServerSocket::ServerSocket(unsigned short port, int max_connect)
		:m_sock(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
		m_max_connect(max_connect),
		m_addr(std::make_unique<sockaddr_in>())
	{
		if (m_sock == INVALID_SOCKET)
			throw std::string("ソケット作成失敗");
		m_addr->sin_family = AF_INET;
		m_addr->sin_port = htons(port);
		m_addr->sin_addr.S_un.S_addr = INADDR_ANY;
	}

	bool ServerSocket::Bind() {
		return bind(m_sock, reinterpret_cast<sockaddr*>(m_addr.get()), sizeof(*m_addr)) == 0;
	}

	bool ServerSocket::Listen()const {
		return listen(m_sock, m_max_connect) == 0;
	}
	std::unique_ptr<SendSocket> ServerSocket::Accept() {
		sockaddr_in client;
		int len = sizeof(client);
		SOCKET socket = accept(m_sock, reinterpret_cast<sockaddr *>(&client), &len);
		if (socket == INVALID_SOCKET)
			return nullptr;
		return std::make_unique<SendSocket>(socket);
	}

	bool ServerSocket::Close() {
		closeFlag = true;
		return  shutdown(m_sock, SD_BOTH) == 0 && closesocket(m_sock) == 0;
	}

	ServerSocket::~ServerSocket()
	{
		if (closeFlag == false)
			Close();
	}
}