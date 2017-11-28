#pragma comment( lib, "ws2_32.lib" )

#include <iostream>
#include <memory>
#include "tcpframework.hpp"

void main() {

	if (tcpframework::TcpManager::Init())
		std::cout << "�������ɐ���" << std::endl;
	auto server = std::make_unique<tcpframework::ServerSocket>(8080, 5);
	if (server->Bind())
		std::cout << "Bind����"<<std::endl;

	if(server->Listen())
		std::cout << "Listen����" << std::endl;
	std::cout << "�ڑ����E�E�E�E" << std::endl;
	auto sendSocket=server->Accept();
	std::cout << "�ڑ�����" << std::endl;
	while (true)
	{
		tcpframework::ByteArray byteArray;
		if (sendSocket->Receive(std::move(byteArray)) == -1) {
			sendSocket->Close();
			break;
		}
		std::string s(byteArray.GetBytes(), byteArray.Size());
		std::cout << s << std::endl;
	}
	tcpframework::TcpManager::End();
}