#pragma once
#include "tuple.hpp"
#include "bit_stream.hpp"
#include "block_specifier.hpp"
using namespace std;

class Block {
	private:
		const Schema& block_schema;
		vector<Tuple> tuple_list;
		int capacity;
		bool pinned;
		bool dirty;
		BlockSpecifier specifier;
		void writeBack() const;
		void clean();

	public:
		Block(const Schema&, const BlockSpecifier&);
		Block(const Block&) = delete;
		Block() = delete;
		~Block();
		vector<Tuple> selectTuple(const vector<Requirement>&) const;
		void insertTuple(const Tuple&);
		void deleteTuple(const vector<Requirement>&);
		void pin();
		void unpin();

};