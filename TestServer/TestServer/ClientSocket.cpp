#include "ClientSocket.hpp"
#include <WinSock2.h>
#include <ws2tcpip.h>


namespace tcpframework {


		//�z�X�g�����o�C�i��ip�ɕϊ�����
	IN_ADDR serverNameToIp(const std::string& serverName) {
		in_addr hoge;
		//ip�������ip�l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ�����
		if (inet_pton(AF_INET, serverName.c_str(), &hoge) != 1) {
			struct addrinfo hints, *res;
			memset(&hints, 0, sizeof(hints));
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_INET;
			//�z�X�g���̎��A�z�X�g������hostent�\���̎擾
			if (getaddrinfo(serverName.c_str(), nullptr, &hints, &res))
				throw std::string("�z�X�g����IP�ɕϊ����s");

			hoge.S_un = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un;
			freeaddrinfo(res);
			return hoge;
		}
		return hoge;
	}

	ClientSocket::ClientSocket(unsigned short port, const std::string & serverIp)
		:
		m_serverData(std::make_unique<sockaddr_in>()),
		m_port(port),
		m_sock(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	{
		if (m_sock == INVALID_SOCKET)
			throw std::string("�\�P�b�g�쐬���s");
		//�ڑ���T�[�o�[�̐ݒ�

		//�A�h���X�t�@�~���ݒ�
		m_serverData->sin_family = AF_INET;
		//�T�[�o�[ip�̐ݒ�
		m_serverData->sin_addr.s_addr = serverNameToIp(serverIp).S_un.S_addr;
		m_serverData->sin_port = htons(port);//�|�[�g�ԍ����r�b�O�G���f�B�A���ɕϊ�

	}

	ClientSocket::~ClientSocket()
	{
		if (closeFlag == false)
			Close();
	}

	void ClientSocket::Connect()const
	{
		//�Ȃ���܂Ń��[�v
		while (connect(m_sock, (LPSOCKADDR)m_serverData.get(), sizeof(*m_serverData)) == SOCKET_ERROR);
	}

	int ClientSocket::Send(const std::string & str)const
	{
		return send(m_sock, str.c_str(), str.size(), 0);
	}

	int ClientSocket::Receive(ByteArray&& bytes)
	{
		//��M�o�b�t�@
		static char recvbuf[RECVSIZE];
		//�����o�C�g�����L�^����ϐ�
		int givebyte;
		//�f�[�^����M
		givebyte = recv(m_sock, recvbuf, sizeof(recvbuf), 0);
		if (givebyte == SOCKET_ERROR || givebyte==0) { return givebyte; }
		bytes = std::move(ByteArray(recvbuf, givebyte));
		return givebyte;
	}

	bool ClientSocket::Close()
	{
		closeFlag = true;
		return shutdown(m_sock, SD_BOTH) == 0 && closesocket(m_sock) == 0;
	}

}