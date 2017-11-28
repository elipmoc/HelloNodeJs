#pragma once
#include <memory>
#include <thread>
#include "def.hpp"
#include "ByteArray.hpp"


namespace tcpframework {

	//���M�p�\�P�b�g�N���X
	class SendSocket {
		const SOCKET m_sock;
		//�\�P�b�g���I���������ǂ���
		bool closeFlag = false;
	public:
		//�ڑ���̃\�P�b�g��n��
		SendSocket(const SOCKET& sock) :m_sock(sock){};
		~SendSocket();

		//�f�[�^���󂯎��
		int Receive(ByteArray&&);

		//Receive�̔񓯊���
		template<class GetResultFunc>
		void ReceiveAsync(GetResultFunc func) {
			std::thread thr([func=func, this]()mutable {
				ByteArray bytes;
				while (true) {
					int byteSize = this->Receive(std::move(bytes));
					if (func(std::move(bytes), byteSize) == false)return;
				}
			});
			thr.detach();
		}

		//�\�P�b�g�I������
		bool Close();

		bool IsClose() const noexcept { return closeFlag; }

		//�f�[�^�𑗐M����i�Ԃ�l�͑��M�����o�C�g���B -1�ŃG���[�j
		int Send(const std::string& str);
	};
}