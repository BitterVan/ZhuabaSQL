#include "bit_stream.hpp"
using namespace std;

BitStream::BitStream(int src_capacity) : capacity(src_capacity), bit_head(new char[src_capacity]) {}

BitStream::BitStream(const BitStream& src_bit_stream) : capacity(src_bit_stream.capacity) {
	bit_head = new char[src_bit_stream.capacity];
	memcpy(bit_head, src_bit_stream.bit_head, capacity);
}

BitStream::~BitStream() {
	delete [] bit_head;
}

char* BitStream::begin() const {
	return bit_head;
}