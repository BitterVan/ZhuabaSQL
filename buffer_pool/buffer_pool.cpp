#include "buffer_pool.hpp"
using namespace std;

BufferPool::BufferPool() {
	ifstream catalog_stream(CATALOG_FILE);
	int schema_amount;
	catalog_stream >> schema_amount;
	string line;
	// This is used to skip the enter character
	getline(catalog_stream, line);
	for (int i = 0; i < schema_amount; i++) {
		getline(catalog_stream, line);
		Schema temp(line);
		schema_map.emplace(temp.name(), temp);
	}
}

BufferPool::~BufferPool() {
	ofstream catalog_stream(CATALOG_FILE);
	catalog_stream << schema_map.size() << endl;
	unpinned_list.clear();
	block_map.clear();
	for (auto i : schema_map) {
		catalog_stream << i.second.toString() << endl;
	}
}


Block& BufferPool::operator[](const BlockSpecifier& src_specifier) {
	// If the block is already in the buffer
	if (block_map.find(src_specifier) != block_map.end()) {
		// If the block is unpinned, move the block to the beginning of the list
		if (find(unpinned_list.begin(), unpinned_list.end(), src_specifier) != unpinned_list.end()) {
			unpinned_list.erase(find(unpinned_list.begin(), unpinned_list.end(), src_specifier));
			unpinned_list.push_front(src_specifier);
		}
		return block_map.find(src_specifier)->second;
	} else {
		// If the buffer is not full, simply add the block to the buffer, and move the block to the beginning of the list
		if (block_map.size() < BUFFER_CAPACITY) {
			unpinned_list.push_front(src_specifier);
			block_map.emplace(src_specifier, Block(schema_map.find(src_specifier.schemaName())->second, src_specifier));
			return block_map.find(src_specifier)->second;
		// If there are unpinned blocks
		} else if (unpinned_list.size()) {
			block_map.erase(unpinned_list.back());
			unpinned_list.pop_back();
			unpinned_list.push_front(src_specifier);
			block_map.emplace(src_specifier, Block(schema_map.find(src_specifier.schemaName())->second, src_specifier));
			return block_map.find(src_specifier)->second;
		} else {
			// return the first one;
			// exceptions will be handled later
			cerr << "No buffer space available" << endl;
			return block_map.begin()->second;
		}
	}
}

void BufferPool::pinBlock(const BlockSpecifier& src_specifier) {
	if (block_map.find(src_specifier) != block_map.end()) {
		if (find(unpinned_list.begin(), unpinned_list.end(), src_specifier) != unpinned_list.end()) {
			unpinned_list.erase(find(unpinned_list.begin(), unpinned_list.end(), src_specifier));
		} else {
			cerr << "Block is aleady pinned" << endl;
		}
	} else {
		cerr << "No such block in the buffer" << endl;
	}
}

void BufferPool::unpinBlock(const BlockSpecifier& src_specifier) {
	if (block_map.find(src_specifier) != block_map.end()) {
		if (find(unpinned_list.begin(), unpinned_list.end(), src_specifier) == unpinned_list.end()) {
			unpinned_list.push_back(src_specifier);
		} else {
			cerr << "Block is aleady pinned" << endl;
		}
	} else {
		cerr << "No such block in the buffer" << endl;
	}
}

void BufferPool::createSchema(const Schema& src_schema) {
	schema_map.emplace(src_schema.name(), src_schema);
}

void BufferPool::dropSchema(const string& src_schema_name) {
	for (auto i = unpinned_list.begin(); i != unpinned_list.end(); i++) {
		if (i->schemaName() == src_schema_name) {
			i = unpinned_list.erase(i);
			i--;
		}
	}
	for (auto i = block_map.begin(); i != block_map.end(); ) {
		if (i->first.schemaName() == src_schema_name) {
			i->second.clean();
			i = block_map.erase(i);
		} else {
			i++;
		}
	}
	if (schema_map.find(src_schema_name) != schema_map.end()) {
		schema_map.erase(schema_map.find(src_schema_name));
	}
	remove(src_schema_name.c_str());
}

int BufferPool::schemaBlockNumber(const string& src_file_name) const {
	fstream file_stream(src_file_name);
	int buffer_max = 0;
	file_stream.seekg(0, file_stream.end);
	for (auto i : block_map) {
		if (i.first.schemaName() == src_file_name) {
			buffer_max = max(buffer_max, i.first.pageNumber()+1);
		}
	}
	return max(int(file_stream.tellg() / BLOCK_SIZE), buffer_max);
}

void BufferPool::_debug_show_info() const {
	cerr << "Schema number: " << schema_map.size() << endl;
	cerr << "Pool block number: " << block_map.size() << endl;
}

const Schema& BufferPool::operator[](const string& src_schema_name) const {
	return schema_map.find(src_schema_name)->second;
}

Type BufferPool::fetchType(const string& src_schema_name, const string& src_attr_name) const {
	auto temp = schema_map.find(src_schema_name)->second;
	auto name_list = temp.nameList();
	auto type_list = temp.typeList();
	for (int i = 0; i < name_list.size(); i++) {
		if (name_list[i] == src_attr_name) {
			return type_list[i];
		}
	}
	return Type::INT;
}