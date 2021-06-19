#include "catalog_manager.hpp"
using namespace std;

CatalogManager::CatalogManager(const CatalogManager&) = default;

CatalogManager::CatalogManager(BufferPool& src_buffer): buffer_pool(src_buffer) {}

CatalogManager::~CatalogManager() = default;

void CatalogManager::createTable(const string& src_schema_name, const vector<string>& src_attr_names, const vector<Attribute>& src_attrs, const string& src_primary_key, const vector<string>& src_unique_name) {
	buffer_pool.createSchema(Schema(src_schema_name, src_attr_names, src_attrs, src_primary_key, src_unique_name));
}

void CatalogManager::dropTable(const string& src_schema_name) {
	buffer_pool.dropSchema(src_schema_name);
}

Type CatalogManager::fetchType(const string& src_schema_name, const string& src_attr_name) const {
	return buffer_pool.fetchType(src_schema_name, src_attr_name);
}

vector<string> CatalogManager::nameList(const string& src_schema_name) const {
	return buffer_pool[src_schema_name].nameList();
}

vector<Type> CatalogManager::typeList(const string& src_schema_name) const {
	return buffer_pool[src_schema_name].typeList();
}

vector<int> CatalogManager::lengthList(const string& src_schema_name) const {
	return buffer_pool[src_schema_name].lengthList();
}

Schema CatalogManager::fetchSchema(const string& src_schema_name) const {
	return buffer_pool[src_schema_name];
}