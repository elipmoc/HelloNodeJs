#include "ClientSocket.hpp"
#include <WinSock2.h>
#include <ws2tcpip.h>


namespace tcpframework {


		//ホスト名をバイナリipに変換する
	IN_ADDR serverNameToIp(const std::string& serverName) {
		in_addr hoge;
		//ip文字列をipネットワークバイトオーダーに変換する
		if (inet_pton(AF_INET, serverName.c_str(), &hoge) != 1) {
			struct addrinfo hints, *res;
			memset(&hints, 0, sizeof(hints));
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_INET;
			//ホスト名の時、ホスト名からhostent構造体取得
			if (getaddrinfo(serverName.c_str(), nullptr, &hints, &res))
				throw std::string("ホスト名をIPに変換失敗");

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
			throw std::string("ソケット作成失敗");
		//接続先サーバーの設定

		//アドレスファミリ設定
		m_serverData->sin_family = AF_INET;
		//サーバーipの設定
		m_serverData->sin_addr.s_addr = serverNameToIp(serverIp).S_un.S_addr;
		m_serverData->sin_port = htons(port);//ポート番号をビッグエンディアンに変換

	}

	ClientSocket::~ClientSocket()
	{
		if (closeFlag == false)
			Close();
	}

	void ClientSocket::Connect()const
	{
		//つながるまでループ
		while (connect(m_sock, (LPSOCKADDR)m_serverData.get(), sizeof(*m_serverData)) == SOCKET_ERROR);
	}

	int ClientSocket::Send(const std::string & str)const
	{
		return send(m_sock, str.c_str(), str.size(), 0);
	}

	int ClientSocket::Receive(ByteArray&& bytes)
	{
		//受信バッファ
		static char recvbuf[RECVSIZE];
		//得たバイト数を記録する変数
		int givebyte;
		//データを受信
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