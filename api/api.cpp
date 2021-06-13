#include "api.hpp"
using namespace std;

API::API(CatalogManager& src_catalog_manager, RecordManager& src_record_manager):
	catalog_manager(src_catalog_manager), record_manager(src_record_manager) {}

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
	return record_manager.selectTuple(src_schema_name, src_requirements);
}

void API::insertTuple(const string& src_schema_name, const vector<string>& src_tuple_vals) {
	record_manager.insertTuple(src_schema_name, src_tuple_vals);
}

void API::deleteTuple(const string& src_schema_name, const vector<Requirement>& src_requirements) {
	record_manager.deleteTuple(src_schema_name, src_requirements);
}

Type API::fetchType(const string& src_schema_name, const string& src_attr_name) const {
	return catalog_manager.fetchType(src_schema_name, src_attr_name);
}