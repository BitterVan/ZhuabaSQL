#pragma once
#include "basics.hpp"
using namespace std;

class BitStream {
	private:
		char* bit_head;
		int capacity;
	
	public:
		BitStream(int);
		BitStream(const BitStream&);
		~BitStream();
		BitStream() = delete;
		char* begin() const;
};