#pragma once
#include "basics.hpp"
#include "item.hpp"
using namespace std;

class Requirement {
	private:
		string attribute_name;
		Item item;
		Operator operator_type;
	
	public:
		Requirement(const string&, const string&, Operator);
		Requirement(const string&, int, Operator);
		Requirement(const string&, double, Operator);
		Requirement(const Requirement&);
		~Requirement();

		friend class Tuple;
};