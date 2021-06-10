#include "schema.hpp"
using namespace std;

Schema::Schema(const Schema&) = default;

Schema::~Schema() = default;

Schema::Schema(const string& src_schema_name, const vector<string>& src_name, const vector<Attribute>& src_map, const string& src_primary_key) :
schema_name(src_schema_name), attribute_name(src_name), primary_key(src_primary_key) {
	for (int i = 0; i < attribute_name.size(); i++) {
		attribute_map.emplace(schema_name[i], src_map[i]);
	}
}

vector<string> Schema::nameList() const {
	return attribute_name;
}

vector<Type> Schema::typeList() const {
	vector<Type> ret;
	for (auto i : attribute_name) {
		ret.push_back(attribute_map.find(i)->second.attributeType());
	}
	return ret;
}

vector<int> Schema::lengthList() const {
	vector<int> ret;
	for (auto i : attribute_name) {
		ret.push_back(attribute_map.find(i)->second.attributeLength());
	}
	return ret;
}

int Schema::bitLength() const {
	vector<int> length_list = lengthList();
	return accumulate(length_list.begin(), length_list.end(), 0);
}

string Schema::name() const {
	return schema_name;
}