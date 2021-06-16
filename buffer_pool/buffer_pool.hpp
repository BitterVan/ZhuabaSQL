#pragma once
#include "../utilities/block.hpp"
using namespace std;

class BufferPool {
	private:
		unordered_map<BlockSpecifier, Block> block_map;
		list<BlockSpecifier> unpinned_list;		
		unordered_map<string, Schema> schema_map;

	public:
		BufferPool();
		~BufferPool();
		BufferPool(const BufferPool&) = delete;

		Block& operator[](const BlockSpecifier&);
		const Schema& operator[](const string&) const;
		void pinBlock(const BlockSpecifier&);
		void unpinBlock(const BlockSpecifier&);
		void dropSchema(const string&);
		void createSchema(const Schema&);
		int schemaBlockNumber(const string&) const;
		void _debug_show_info() const;
		Type fetchType(const string&, const string&) const;

		unordered_map<string, list<int>> free_list;
};