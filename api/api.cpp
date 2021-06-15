#include "api.hpp"
using namespace std;

API::API(CatalogManager& src_catalog_manager, RecordManager& src_record_manager, UI& src_ui):
	catalog_manager(src_catalog_manager), record_manager(src_record_manager), ui(src_ui) {}

API::~API() = default;

void API::createIndex(const string&, const string&, const string&) {
	cerr << "Creating index" << endl;
}

void API::dropIndex(const string&) {
	cerr << "Droping index" << endl;
}

void API::createTable(const string& src_schema_name, const vector<string>& src_attr_names, const vector<Attribute>& src_attrs, const string& src_primary_key) {
	catalog_manager.createTable(src_schema_name, src_attr_names, src_attrs, src_primary_key);	
}

void API::dropTable(const string& src_schema_name) {
	catalog_manager.dropTable(src_schema_name);	
}

vector<Tuple> API::selectTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) const {
	auto nameList = catalog_manager.nameList(src_schema_name);
	auto typeList = catalog_manager.typeList(src_schema_name);
	auto lengthList = catalog_manager.lengthList(src_schema_name);
	auto ret = record_manager.selectTuple(src_schema_name, src_requirements);
	ui.plotTable(nameList, typeList, lengthList, ret, nameList);
	return ret;
}

void API::insertTuple(const string& src_schema_name, const vector<string>& src_tuple_vals) {
	auto schema = catalog_manager.fetchSchema(src_schema_name);
	auto primary_key = schema.primaryKey();
	auto type = catalog_manager.fetchType(src_schema_name, primary_key);
	auto nameList = catalog_manager.nameList(src_schema_name);
	int pos = find(nameList.begin(), nameList.end(), primary_key) - nameList.begin();
	vector<Requirement> require;
	// cerr << src_tuple_vals[pos] << endl;
	string temp = string(src_tuple_vals[pos], 1, src_tuple_vals[pos].size() - 2);
	switch (type)
	{
	case Type::INT:
		require.push_back(Requirement(primary_key, Item(str2int(src_tuple_vals[pos])), Operator::EQ));
		break;
	case Type::DOUBLE:
		require.push_back(Requirement(primary_key, Item(str2double(src_tuple_vals[pos])), Operator::EQ));
		break;
	case Type::STRING:
		require.push_back(Requirement(primary_key, Item(temp), Operator::EQ));
		break;
	default:
		break;
	}
	auto select = selectTuple(src_schema_name, require);
	if (select.size()) {
		throw PrimaryExist();
	}
	record_manager.insertTuple(src_schema_name, src_tuple_vals);
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