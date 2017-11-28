#pragma once
#include <memory>

namespace tcpframework {

	//Winsock2API�̏������ƏI���̏���������N���X
	class TcpManager {
		class TcpManager_impl;
		std::unique_ptr<TcpManager_impl> impl;
		TcpManager();
		static TcpManager* instance;
	public:
		//API������
		static bool Init();
		//API�I������
		static void End();
	};
}