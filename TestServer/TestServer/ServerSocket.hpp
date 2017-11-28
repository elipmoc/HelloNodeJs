#pragma once
#include "def.hpp"
#include <memory>
namespace tcpframework {

	class SendSocket;

	//�T�[�o�ŃN���C�A���g�̑ҋ@�Ɏg���\�P�b�g�N���X
	class ServerSocket {
		SOCKET m_sock;
		std::unique_ptr<sockaddr_in> m_addr;
		const int m_max_connect;
		//�\�P�b�g�I���t���O
		bool closeFlag = false;
	public:
		ServerSocket(unsigned short port, int max_connect);

		~ServerSocket();

		//�\�P�b�g�Ƀ|�[�g�A�A�h���X�t�@�~���[�A�T�[�o�[��IP��R�Â�
		bool Bind();

		//�\�P�b�g�̐ڑ��ҋ@����������
		bool Listen()const;

		//�ڑ���ҋ@����B�ڑ����ꂽ�炻�̐ڑ���̃\�P�b�g��Ԃ�
		std::unique_ptr<SendSocket> Accept();

		//Accept�̔񓯊���
		template<class GetSendSocketFunc>
		void  AcceptAsync(GetSendSocketFunc func){
			std::thread thr([func=func, this]() mutable{
				while (true)
					if (func(this->Accept()) == false)return;
			});
			thr.detach();
		};

		//�\�P�b�g�I������
		bool Close();
	};

}