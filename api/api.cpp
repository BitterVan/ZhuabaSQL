#include "api.hpp"
using namespace std;

API::API(CatalogManager& src_catalog_manager, IndexManager& src_index_manager, RecordManager& src_record_manager, UI& src_ui):
	catalog_manager(src_catalog_manager), index_manager(src_index_manager), record_manager(src_record_manager), ui(src_ui) {}

API::~API() = default;

void API::createIndex(const string& src_table_name, const string& src_schema_name, const string& src_attr_name) {
	index_manager.createIndex(src_table_name, src_schema_name, src_attr_name);
}

void API::dropIndex(const string& src_index_name) {
	index_manager.dropIndex(src_index_name);
}

void API::createTable(const string& src_schema_name, const vector<string>& src_attr_names, const vector<Attribute>& src_attrs, const string& src_primary_key, const vector<string>& src_unique_name) {
	catalog_manager.createTable(src_schema_name, src_attr_names, src_attrs, src_primary_key, src_unique_name);	
}

void API::dropTable(const string& src_schema_name) {
	catalog_manager.dropTable(src_schema_name);	
}

vector<Tuple> API::selectTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) const {
	vector<Tuple> ret;
	// If the first requirement is EQ, and is in the index
	if (src_requirements.size() > 0 && index_manager.holdIndex(src_schema_name, src_requirements[0].attribute_name + src_schema_name)&&src_requirements[0].operator_type == Operator::EQ) {
		ret = index_manager.selectTuple(src_schema_name, src_requirements[0]);
	} else {
		ret = record_manager.selectTuple(src_schema_name, src_requirements);
	}
	auto nameList = catalog_manager.nameList(src_schema_name);
	auto typeList = catalog_manager.typeList(src_schema_name);
	auto lengthList = catalog_manager.lengthList(src_schema_name);
	ui.plotTable(nameList, typeList, lengthList, ret, nameList);
	return ret;
}

void API::insertTuple(const string& src_schema_name, const vector<string>& src_tuple_vals) {
	auto itemp = record_manager.insertTuple(src_schema_name, src_tuple_vals);
	auto name_list = catalog_manager.nameList(src_schema_name);
	auto type_list = catalog_manager.typeList(src_schema_name);
	for (int i = 0; i < name_list.size(); i++) {
		if (index_manager.holdIndex(src_schema_name, name_list[i])) {
				switch (type_list[i])
				{
				case Type::INT:
					index_manager.insertIndex(src_schema_name, name_list[i], Item(str2int(src_tuple_vals[i])), itemp);
					break;
				case Type::DOUBLE:
					index_manager.insertIndex(src_schema_name, name_list[i], Item(str2double(src_tuple_vals[i])), itemp);
					break;
				case Type::STRING:
					index_manager.insertIndex(src_schema_name, name_list[i], Item(str2str(src_tuple_vals[i])), itemp);
					break;			
				default:
					break;
				}
		}
	}
	// for (auto i : name_list) {
	// 	if (index_manager.holdIndex(src_schema_name, i)) {
	// 		for (int j = 0; j < type_list.size(); j++) {
	// 			switch (type_list[j])
	// 			{
	// 			case Type::INT:
	// 				index_manager.insertIndex(src_schema_name, name_list[j], Item(str2int(src_tuple_vals[j])), itemp);
	// 				break;
	// 			case Type::DOUBLE:
	// 				index_manager.insertIndex(src_schema_name, name_list[j], Item(str2double(src_tuple_vals[j])), itemp);
	// 				break;
	// 			case Type::STRING:
	// 				index_manager.insertIndex(src_schema_name, name_list[j], Item(str2str(src_tuple_vals[j])), itemp);
	// 				break;			
	// 			default:
	// 				break;
	// 			}
	// 		}
	// 	}
	// }
}

void API::deleteTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) {
	record_manager.deleteTuple(src_schema_name, src_requirements);
}

Type API::fetchType(const string& src_schema_name, const string& src_attr_name) const {
	return catalog_manager.fetchType(src_schema_name, src_attr_name);
}

string API::fetchStatement() const {
	return ui.fetchStatement();
}

vector<string> API::fetchFile(const string& src_file_name) const {
	vector<string> ret;
	string temp;
	ifstream file_stream(src_file_name);
	while (getline(file_stream, temp)) {
		ret.push_back(temp);
	}
	file_stream.close();
	return ret;
}