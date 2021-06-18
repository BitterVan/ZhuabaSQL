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
		// bool pinned;
		bool dirty;
		BlockSpecifier specifier;
		void writeBack() const;

	public:
		Block(const Schema&, const BlockSpecifier&);
		Block(const Block&);
		Block() = delete;
		~Block();
		vector<Tuple> selectTuple(const vector<Requirement>&) const;
		int insertTuple(const Tuple&);
		void orderedInsert(const Tuple&);
		void deleteTuple(const vector<Requirement>&);
		void clean();
		bool isFull() const;
		string schemaName() const;
		void flowInto(Block&);
		bool holdingLeaf() const;
		// void pin();
		// void unpin();

};