#include "record_manager.hpp"
using namespace std;

RecordManager::RecordManager(const RecordManager&) = default;

RecordManager::RecordManager(BufferPool& src_buffer) : buffer_pool(src_buffer) {}

RecordManager::~RecordManager() = default;

TupleSpecifier RecordManager::insertTuple(const string& src_schema_name, const vector<string>& src_tuple_vals) {
	// test if the hash is generated
	auto unique_list = buffer_pool[src_schema_name].uniqueList();
	if (unique_list.size() > 0 && buffer_pool.duplicant_map[src_schema_name + unique_list[0]].size() == 0) {
		auto res = selectTuple(src_schema_name, vector<Requirement>());
		for (auto i : res) {
			auto temp = i.valueList(unique_list);
			for (int j = 0; j < unique_list.size(); j++) {
				buffer_pool.duplicant_map[src_schema_name + unique_list[j]][temp[j]] = 1;
			}
		}
	}

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
	// int cnt = 0;
	// for (int i = buffer_pool.schemaBlockNumber(src_schema_name) - 1; i >= 0; i--) {
	// 	cnt++;
	// 	if (cnt > 2) break;
	// 	if (!buffer_pool[BlockSpecifier(src_schema_name, i)].isFull()) {
	// 		buffer_pool[BlockSpecifier(src_schema_name, i)].insertTuple(Tuple(buffer_pool[src_schema_name], items));
	// 		return;
	// 	}
	// }
	Tuple tuple(buffer_pool[src_schema_name], items);
	auto item_list = tuple.valueList(unique_list);
	for (int i = 0; i < item_list.size(); i++) {
		if (buffer_pool.duplicant_map[src_schema_name + unique_list[i]][item_list[i]] == 1) {
			throw DuplicantUnique();
			break;
		}
		buffer_pool.duplicant_map[src_schema_name + unique_list[i]][item_list[i]] = 1;
	}

	int first_free = buffer_pool.free_list[src_schema_name].front();
	int pos = buffer_pool[BlockSpecifier(src_schema_name, first_free)].insertTuple(tuple);
	if (buffer_pool[BlockSpecifier(src_schema_name, first_free)].isFull()) {
		buffer_pool.free_list[src_schema_name].pop_front();
	}
	return TupleSpecifier(src_schema_name, first_free, pos);
	// buffer_pool[BlockSpecifier(src_schema_name, buffer_pool.schemaBlockNumber(src_schema_name))].insertTuple(Tuple(buffer_pool[src_schema_name], items));
} 

void RecordManager::deleteTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) {

	auto unique_list = buffer_pool[src_schema_name].uniqueList();
	if (unique_list.size() > 0 && buffer_pool.duplicant_map[src_schema_name + unique_list[0]].size() == 0) {
		auto res = selectTuple(src_schema_name, vector<Requirement>());
		for (auto i : res) {
			auto temp = i.valueList(unique_list);
			for (int j = 0; j < unique_list.size(); j++) {
				buffer_pool.duplicant_map[src_schema_name + unique_list[j]][temp[j]] = 1;
			}
		}
	}

	for (int i = 0; i < buffer_pool.schemaBlockNumber(src_schema_name); i++) {
		auto res = buffer_pool[BlockSpecifier(src_schema_name, i)].deleteTuple(src_requirements);
		for (auto i : res) {
			auto temp = i.valueList(unique_list);
			for (int j = 0; j < unique_list.size(); j++) {
				buffer_pool.duplicant_map[src_schema_name + unique_list[j]][temp[j]] = 0;
			}
		}
		
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

vector<TupleSpecifier> RecordManager::fetchAllSpecifier(const string& src_schema_name) {
	vector<TupleSpecifier> ret;
	// cerr << buffer_pool.schemaBlockNumber(src_schema_name) << endl;
	for (int i = 0; i < buffer_pool.schemaBlockNumber(src_schema_name); i++) {
		auto temp = buffer_pool[BlockSpecifier(src_schema_name, i)].validSpecifiers();
		// cerr << temp.size() << endl;
		for (auto j : temp) {
			ret.push_back(j);
		}
	}
	return ret;
}