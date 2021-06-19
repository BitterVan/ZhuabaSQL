#pragma once
#include "attribute.hpp"
#include "numeric"
using namespace std;

class Schema {
	private:
		string schema_name;
		vector<string> attribute_name;
		vector<string> unique_name;
		unordered_map<string, Attribute> attribute_map;
		string primary_key;

	public:
		Schema() = delete;
		Schema(const Schema&);
		~Schema();
		Schema(const string&, const vector<string>&, const vector<Attribute>&, const string&, const vector<string>&);
		Schema(const string&);
		Schema& operator=(const Schema&);
		vector<string> nameList() const;
		vector<Type> typeList() const;
		vector<int> lengthList() const;
		vector<string> uniqueList() const;
		int bitLength() const;
		string name() const;
		string primaryKey() const;
		string toString() const;
		void _debug_show_info() const;
		string fisrtAttribute() const;
};