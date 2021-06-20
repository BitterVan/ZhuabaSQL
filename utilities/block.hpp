#pragma once
#include "tuple.hpp"
#include "bit_stream.hpp"
#include "block_specifier.hpp"
using namespace std;

class Block {
	private:
		const Schema& block_schema;
		int capacity;
		// bool pinned;
		bool dirty;
		BlockSpecifier specifier;
		void writeBack() const;

	public:
		vector<Tuple> tuple_list;
		Block(const Schema&, const BlockSpecifier&);
		Block(const Block&);
		Block() = delete;
		~Block();
		vector<Tuple> selectTuple(const vector<Requirement>&) const;
		int insertTuple(const Tuple&);
		void orderedInsert(const Tuple&);
		vector<Tuple> deleteTuple(const vector<Requirement>&);
		void clean();
		bool isFull() const;
		string schemaName() const;
		void flowInto(Block&);
		bool holdingLeaf() const;
		BlockSpecifier findPosition(const string&, const Tuple&) const;
		BlockSpecifier parentPosition() const;
		BlockSpecifier previousPosition() const;
		Tuple parentTuple() const;
		bool holdRoot() const;
		Tuple goingUpTuple() const;
		void commitDad(const BlockSpecifier&);
		vector<TupleSpecifier> validSpecifiers() const;
		
		friend class IndexManager;
		friend class BufferPool;
		// void pin();
		// void unpin();

};