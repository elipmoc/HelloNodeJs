#include "SendSocket.hpp"
#include <string>
#include <queue>
#include <mutex>
#include <WinSock2.h>

namespace tcpframework {

	SendSocket::~SendSocket()
	{
		if (closeFlag == false)
			Close();
	}

	int SendSocket::Receive(ByteArray&& bytes)
	{
		//��M�o�b�t�@
		static char recvbuf[RECVSIZE];
		//�����o�C�g�����L�^����ϐ�
		int byteSize;
		//�f�[�^����M
		byteSize = recv(m_sock, recvbuf, sizeof(recvbuf), 0);
		if (byteSize == SOCKET_ERROR || byteSize == 0) { return byteSize; }

		bytes = ByteArray(recvbuf, byteSize);
		return byteSize;
	}

	bool SendSocket::Close()
	{
		closeFlag = true;
		return shutdown(m_sock, SD_BOTH) == 0 && closesocket(m_sock) == 0;
	}

	int SendSocket::Send(const std::string & str)
	{
		return send(m_sock, str.c_str(), str.size(), 0);
	}

}