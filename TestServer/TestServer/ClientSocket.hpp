#pragma once
#include <memory>
#include <string>
#include "def.hpp"
#include "ByteArray.hpp"

namespace tcpframework {
	class ClientSocket {
		SOCKET m_sock;
		std::unique_ptr<sockaddr_in> m_serverData;
		const unsigned short m_port;
		//ソケット終了フラグ
		bool closeFlag = false;
	public:

		ClientSocket(unsigned short port,const std::string& serverIp);

		~ClientSocket();

		//接続を待機する。
		void Connect()const;

		//Connectの非同期版
		template<class ConnectedFunc>
		void ConnectAsync(ConnectedFunc func)const {
			std::thread thr([func = func, this]() mutable {
				Connect();
				func();
			});
			thr.detach();
		}

		//データを送信する
		int Send(const std::string&)const;

		//データをBufに貯める
		//返り値はバイト数で
		//エラーなら-1が返る
		int Receive(ByteArray&&);

		//Receiveの非同期版
		template<class GetResultFunc>
		void ReceiveAsync(GetResultFunc func) {
			std::thread thr([func = func, this]()mutable {
				ByteArray bytes;
				while (true) {
					int byteSize = this->Receive(std::move(bytes));
					if (func(std::move(bytes), byteSize) == false)return;
				}
			});
			thr.detach();
		}


		//ソケットを終了する
		bool Close();
	};
}