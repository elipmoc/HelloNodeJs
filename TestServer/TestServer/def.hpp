#pragma once


//������winsock�Œ�`����Ă���^��O���錾
#ifdef	_WIN64
	typedef unsigned __int64 UINT_PTR;
#elif defined _WIN32
	typedef unsigned int UINT_PTR;
#endif 

	typedef UINT_PTR SOCKET;
	struct sockaddr_in;
