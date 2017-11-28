#pragma once

namespace tcpframework {

	//受信バッファサイズ
	const size_t RECVSIZE = 65536;
	//SoketのReceiveで受け取るバイト配列
	class ByteArray {

		size_t m_size;
		char* m_bytes;

	public:

		ByteArray(char const*const byte, size_t size) :m_size(size) {
			m_bytes = new char[size];
			for (size_t i = 0; i < size; i++) {
				m_bytes[i] = byte[i];
			}
		}

		ByteArray() :m_size(0), m_bytes(nullptr) {}

		ByteArray(const ByteArray&) = delete;

		ByteArray(ByteArray&& move) :m_size(move.m_size) {
			m_bytes = move.m_bytes;
			move.m_bytes = nullptr;
			move.m_size = 0;
		}

		void operator =(ByteArray&& movecp) {
			Clear();
			m_size = movecp.m_size;
			m_bytes = movecp.m_bytes;
			movecp.m_bytes = nullptr;
			movecp.m_size = 0;
		}

		~ByteArray() { Clear(); }

		void Clear() {
			if (m_bytes != nullptr)
				delete[] m_bytes;
		}

		size_t Size()const noexcept { return m_size; }

		char const * const GetBytes()const noexcept { return m_bytes; }
	};

}