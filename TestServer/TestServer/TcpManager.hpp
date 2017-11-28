#pragma once
#include <memory>

namespace tcpframework {

	//Winsock2APIの初期化と終了の処理をするクラス
	class TcpManager {
		class TcpManager_impl;
		std::unique_ptr<TcpManager_impl> impl;
		TcpManager();
		static TcpManager* instance;
	public:
		//API初期化
		static bool Init();
		//API終了処理
		static void End();
	};
}