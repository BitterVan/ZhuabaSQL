#include "index_manager.hpp"
using namespace std;

IndexManager::IndexManager(BufferPool& src_buffer_pool, CatalogManager& src_catalog_manager, RecordManager& src_record_manager):
buffer_pool(src_buffer_pool), catalog_manager(src_catalog_manager), record_manager(src_record_manager) {
	// do nothing
	ifstream temp(FILE_PREFIX + ".index_catalog");
	int n;
	temp >> n;
	string name, index_name;
	int root_pos;
	for (int i = 0; i < n; i++) {
		temp >> name >> index_name >> root_pos;
		index_map.emplace(name, index_name);
		root_position.emplace(name, root_pos);
	}	
	temp.close();
}

IndexManager::~IndexManager() {
	ofstream temp(FILE_PREFIX + ".index_catalog");
	temp << index_map.size() << endl;
	for (auto i : index_map) {
		temp << i.first << " " << i.second << " ";
		temp << root_position.find(i.first)->second << endl;
	}
}

bool IndexManager::holdIndex(const string& src_schema_name, const string& src_attr_name) const {
	try {
		buffer_pool.fetchType(src_schema_name+src_attr_name, "key");
	} catch (exception& exp) {
		return 0;
	}
	return 1;
}

void IndexManager::createIndex(const string& src_schema_name, const string& src_index_name, const string& src_attr_name) {
	buffer_pool.fetchType(src_schema_name, src_attr_name);
	index_map[src_index_name] = src_schema_name + src_attr_name;
	root_position[src_index_name] = 0;
	auto type = buffer_pool.fetchType(src_schema_name, src_attr_name);
	auto length = buffer_pool.fetchLength(src_schema_name, src_attr_name);
	vector<Attribute> attr_list;
	attr_list.push_back(Attribute(type, length));
	attr_list.push_back(Attribute(Type::STRING, (src_schema_name + src_attr_name).length()));
	attr_list.push_back(Attribute(Type::INT, sizeof(int)));
	attr_list.push_back(Attribute(Type::INT, sizeof(int)));
	vector<string> name_list;
	name_list.push_back("key");
	name_list.push_back("file_name");
	name_list.push_back("block_number");
	name_list.push_back("tuple_number");
	catalog_manager.createTable(src_schema_name+src_attr_name, name_list, attr_list, INVALID_PRIMARY);
}

