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
		void pinBlock(const BlockSpecifier&);
		void unpinBlock(const BlockSpecifier&);
		void dropSchema(const string&);
		void createSchema(const Schema&);
		int schemaBlockNumber(const string&) const;
		void _debug_show_info() const;
};