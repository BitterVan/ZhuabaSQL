#pragma once
#include "basics.hpp"
using namespace std;

class BitStream {
	private:
		char* bit_head;
		int capacity;
	
	public:
		BitStream();
		BitStream(const BitStream&);
		~BitStream();
		char* begin() const;
};