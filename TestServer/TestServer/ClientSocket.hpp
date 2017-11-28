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
		//�\�P�b�g�I���t���O
		bool closeFlag = false;
	public:

		ClientSocket(unsigned short port,const std::string& serverIp);

		~ClientSocket();

		//�ڑ���ҋ@����B
		void Connect()const;

		//Connect�̔񓯊���
		template<class ConnectedFunc>
		void ConnectAsync(ConnectedFunc func)const {
			std::thread thr([func = func, this]() mutable {
				Connect();
				func();
			});
			thr.detach();
		}

		//�f�[�^�𑗐M����
		int Send(const std::string&)const;

		//�f�[�^��Buf�ɒ��߂�
		//�Ԃ�l�̓o�C�g����
		//�G���[�Ȃ�-1���Ԃ�
		int Receive(ByteArray&&);

		//Receive�̔񓯊���
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


		//�\�P�b�g���I������
		bool Close();
	};
}