#include "schema.hpp"
using namespace std;

Schema::Schema(const Schema&) = default;

Schema::~Schema() = default;

Schema::Schema(const string& src_string) {
	stringstream src_stream(src_string);
	src_stream >> schema_name;
	int size;
	string temp;
	int temp_type, temp_len;
	src_stream >> size;
	for (int i = 0; i < size; i++) {
		src_stream >> temp;
		attribute_name.push_back(temp);
	}
	for (int i = 0; i < size; i++) {
		src_stream >> temp_type >> temp_len;
		attribute_map.emplace(attribute_name[i], Attribute(Type(temp_type), temp_len));
	}
	src_stream >> primary_key;
	int n;
	src_stream >> n;
	for (int i = 0; i < n; i++) {
		src_stream >> temp;
		unique_name.push_back(temp);
	}
} 

Schema::Schema(const string& src_schema_name, const vector<string>& src_name, const vector<Attribute>& src_map, const string& src_primary_key, const vector<string>& src_unique_name) :
schema_name(src_schema_name), attribute_name(src_name), primary_key(src_primary_key), unique_name(src_unique_name) {
	for (int i = 0; i < attribute_name.size(); i++) {
		attribute_map.emplace(src_name[i], src_map[i]);
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
	return accumulate(length_list.begin(), length_list.end(), sizeof(char));
}

string Schema::name() const {
	return schema_name;
}

void Schema::_debug_show_info() const {
	cerr << "Schema name: " << schema_name << endl;
	for (auto i : attribute_name) {
		cerr << i << ": " << int(attribute_map.find(i)->second.attributeType()) << " " << attribute_map.find(i)->second.attributeLength() << endl;
	}
	cerr << "Primary key: " << primary_key << endl;
	cerr << "Bit Length: " << bitLength() << endl;
}

Schema& Schema::operator=(const Schema&) = default;

string Schema::toString() const {
	string ret;
	ret += schema_name + " ";
	ret += to_string(attribute_map.size()) + " ";
	for (auto i : attribute_name) {
		ret += i + " ";
	}
	for (auto i : attribute_name) {
		ret += to_string(int(attribute_map.find(i)->second.attributeType())) + " ";
		ret += to_string(int(attribute_map.find(i)->second.attributeLength())) + " ";
	}
	ret += primary_key + " ";
	ret += to_string(unique_name.size()) + " ";
	for (auto i : unique_name) {
		ret += i + " ";
	}
	return ret;
}

string Schema::primaryKey() const {
	return primary_key;
}

string Schema::fisrtAttribute() const {
	return attribute_name[0];
}

vector<string> Schema::uniqueList() const {
	return unique_name;
}