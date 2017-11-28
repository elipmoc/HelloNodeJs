#include "TcpManager.hpp"
#include <WinSock2.h>

namespace tcpframework {
	class TcpManager::TcpManager_impl {
		WSADATA m_wsa_data;
	public:

		//API初期化処理
		bool Init() {
			int error = WSAStartup(MAKEWORD(2, 0), &m_wsa_data);
			if (error != 0)return false;
			return true;
		}

		void End() {
			//API終了処理
			WSACleanup();
		}
	};

	//static変数のインスタンス化
	TcpManager* TcpManager::instance = nullptr;

	TcpManager::TcpManager()
		:impl(std::make_unique<TcpManager_impl>())
	{}

	bool TcpManager::Init()
	{
		if (instance != nullptr)return false;
		instance = new TcpManager();
		return instance->impl->Init();
	}

	void TcpManager::End()
	{
		instance->impl->End();
		delete instance;
	}

}