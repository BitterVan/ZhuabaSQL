#include "record_manager.hpp"
using namespace std;

RecordManager::RecordManager(const RecordManager&) = default;

RecordManager::RecordManager(BufferPool& src_buffer) : buffer_pool(src_buffer) {}

RecordManager::~RecordManager() = default;

void RecordManager::insertTuple(const string& src_schema_name, const vector<string>& src_tuple_vals) {
	auto primary_key = buffer_pool[src_schema_name].primaryKey();
	auto primary_type = buffer_pool.fetchType(src_schema_name, primary_key);
	auto name_list = buffer_pool[src_schema_name].nameList();
	int position = find(name_list.begin(), name_list.end(), primary_key) - name_list.begin();
	if (src_tuple_vals[position] == "null") {
		throw PrimaryNull();
	}
	auto type_list = buffer_pool[src_schema_name].typeList();
	if (type_list.size() != src_tuple_vals.size()) {
		throw TupleInitialFail();
	}
	vector<Item> items;
	string temp;
	for (int i = 0; i < type_list.size(); i++) {
		switch (type_list[i]) {
		case Type::INT:
			items.push_back(Item(str2int(src_tuple_vals[i])));
			break;
		case Type::DOUBLE:
			items.push_back(Item(str2double(src_tuple_vals[i])));
			break;
		case Type::STRING:
			if (src_tuple_vals[i][0] == '\'') {
				temp = string(src_tuple_vals[i], 1, src_tuple_vals[i].length() - 2);	
			} else if (src_tuple_vals[i] == "null"){
				temp = "null";
			}
			items.push_back(Item(temp));
			break;
		default:
			break;
		}
	}
	for (int i = 0; i <= buffer_pool.schemaBlockNumber(src_schema_name); i++) {
		if (!buffer_pool[BlockSpecifier(src_schema_name, i)].isFull()) {
			buffer_pool[BlockSpecifier(src_schema_name, i)].insertTuple(Tuple(buffer_pool[src_schema_name], items));
			break;
		}
	}
} 

void RecordManager::deleteTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) {
	for (int i = 0; i < buffer_pool.schemaBlockNumber(src_schema_name); i++) {
		buffer_pool[BlockSpecifier(src_schema_name, i)].deleteTuple(src_requirements);
	}
}

vector<Tuple> RecordManager::selectTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) const {
	vector<Tuple> ret;
	// cerr << buffer_pool.schemaBlockNumber(src_schema_name) << endl;
	for (int i = 0; i < buffer_pool.schemaBlockNumber(src_schema_name); i++) {
		auto temp = buffer_pool[BlockSpecifier(src_schema_name, i)].selectTuple(src_requirements);
		// cerr << temp.size() << endl;
		for (auto j : temp) {
			ret.push_back(j);
		}
	}
	return ret;
}