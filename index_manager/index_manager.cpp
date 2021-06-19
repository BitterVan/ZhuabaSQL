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
	temp.close();
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
	string real_name = src_schema_name + src_attr_name;
	buffer_pool.fetchType(src_schema_name, src_attr_name);
	index_map[src_index_name] = real_name;
	root_position[real_name] = 0;
	auto type = buffer_pool.fetchType(src_schema_name, src_attr_name);
	auto length = buffer_pool.fetchLength(src_schema_name, src_attr_name);
	vector<Attribute> attr_list;
	attr_list.push_back(Attribute(type, length));
	attr_list.push_back(Attribute(Type::STRING, max(int((src_schema_name + src_attr_name).length()), length)));
	attr_list.push_back(Attribute(Type::INT, sizeof(int)));
	attr_list.push_back(Attribute(Type::INT, sizeof(int)));
	vector<string> name_list;
	name_list.push_back("key");
	name_list.push_back("file_name");
	name_list.push_back("block_number");
	name_list.push_back("tuple_number");
	catalog_manager.createTable(real_name, name_list, attr_list, INVALID_PRIMARY, vector<string>());
	vector<Item> first_val;
	vector<Item> second_val;
	switch (type) {
	case Type::INT:
		first_val.push_back(Item(PARENT_INT));
		second_val.push_back(Item(MIN_INT));
		break;
	case Type::DOUBLE:
		first_val.push_back(Item(PARENT_DOUBLE));
		second_val.push_back(Item(MIN_DOUBLE));
		break;
	case Type::STRING:
		first_val.push_back(Item(PARENT_STRING));
		second_val.push_back(Item(MIN_STRING));	
		break;
	default:
		break;
	}

	first_val.push_back(Item(ROOT_PARENT_FILE_NAME));
	first_val.push_back(Item(ROOT_PARENT_PAGE_NUMBER));
	first_val.push_back(Item(INVALID_TUPLE_NUMBER));
	second_val.push_back(Item(real_name));
	second_val.push_back(Item(MIN_PAGE_NUMBER));
	second_val.push_back(Item(INVALID_TUPLE_NUMBER));
	
	buffer_pool[BlockSpecifier(real_name, 0)].insertTuple(Tuple(buffer_pool[real_name], first_val));
	buffer_pool[BlockSpecifier(real_name, 0)].insertTuple(Tuple(buffer_pool[real_name], second_val));
}

void IndexManager::insertIndex(const string& src_schema_name, const string& src_attr_name, const Item& src_item, const TupleSpecifier& src_specifier) {
	string real_name = src_schema_name + src_attr_name;
	Block* now = &(buffer_pool[BlockSpecifier(real_name, root_position[real_name])]);
	vector<Item> attrs;
	attrs.push_back(src_item);
	attrs.push_back(src_schema_name);
	attrs.push_back(src_specifier.page_number);
	attrs.push_back(src_specifier.tuple_number);
	auto tuple = Tuple(buffer_pool[real_name], attrs);
	while (true) {
		if (now->holdingLeaf()) {
			now->orderedInsert(tuple);
			break;
		} else {
			now = &(buffer_pool[now->findPosition(src_attr_name, tuple)]);
		}
	}
	while (true) {
		if (now->isFull()) {
			if (now->holdRoot()) {
				root_position[real_name] = buffer_pool.schemaBlockNumber(real_name);
				Block& new_root = buffer_pool.fetchNew(real_name);
				Block& temp = buffer_pool.fetchNew(real_name);
				now->flowInto(temp);
				new_root.insertTuple(now->parentTuple());
				new_root.insertTuple(now->goingUpTuple());
				new_root.insertTuple(temp.goingUpTuple());
				now->commitDad(new_root.specifier);
				temp.commitDad(new_root.specifier);
				break;
			} else {
				Block& temp = buffer_pool.fetchNew(real_name);
				now->flowInto(temp);
				now = &(buffer_pool[now->parentPosition()]);
				now->orderedInsert(temp.goingUpTuple());
			}
		} else {
			break;
		}
	}
}