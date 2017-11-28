#pragma once
#include "def.hpp"
#include <memory>
namespace tcpframework {

	class SendSocket;

	//サーバでクライアントの待機に使うソケットクラス
	class ServerSocket {
		SOCKET m_sock;
		std::unique_ptr<sockaddr_in> m_addr;
		const int m_max_connect;
		//ソケット終了フラグ
		bool closeFlag = false;
	public:
		ServerSocket(unsigned short port, int max_connect);

		~ServerSocket();

		//ソケットにポート、アドレスファミリー、サーバーのIPを紐づけ
		bool Bind();

		//ソケットの接続待機準備をする
		bool Listen()const;

		//接続を待機する。接続されたらその接続先のソケットを返す
		std::unique_ptr<SendSocket> Accept();

		//Acceptの非同期版
		template<class GetSendSocketFunc>
		void  AcceptAsync(GetSendSocketFunc func){
			std::thread thr([func=func, this]() mutable{
				while (true)
					if (func(this->Accept()) == false)return;
			});
			thr.detach();
		};

		//ソケット終了処理
		bool Close();
	};

}