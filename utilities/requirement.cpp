#include "requirement.hpp"
using namespace std;

Requirement::Requirement(const string& src_attr, const Item& src_val, Operator src_type) : attribute_name(src_attr), item(src_val), operator_type(src_type) {}

Requirement::Requirement(const Requirement&) = default;

Requirement::Requirement(const string& src_attr_name, const string& src_operator, const string& src_val, Type src_type) {
	attribute_name = src_attr_name;
	switch (src_type)
	{
	case Type::INT:
		item = Item(str2int(src_val));
		break;
	case Type::DOUBLE:
		item = Item(str2double(src_val));
		break;
	case Type::STRING:
		item = Item(src_val);
		break;	
	default:
		break;
	}
	if (src_operator == "=") {
		operator_type = Operator::EQ;
	} else if (src_operator == "<=") {
		operator_type = Operator::LE;
	} else if (src_operator == "<") {
		operator_type = Operator::LT;
	} else if (src_operator == ">") {
		operator_type = Operator::GT;
	} else if (src_operator == ">=") {
		operator_type = Operator::GE;
	} else if (src_operator == "<>") {
		operator_type = Operator::NE;
	} else {
		operator_type = Operator::EQ;
	}
}

Requirement::~Requirement() = default;