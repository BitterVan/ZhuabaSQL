#include "tuple.hpp"
using namespace std;

Tuple::Tuple(const Tuple&) = default;

Tuple::~Tuple() = default;

Tuple::Tuple(const Schema& src_schema, const vector<Item>& src_vals) : tuple_schema(src_schema) {
	valid = 1;
	vector<string> name_list = src_schema.nameList();
	for (int i = 0; i < name_list.size(); i++) {
		tuple_vals.emplace(name_list[i], src_vals[i]);
	}
}

Tuple& Tuple::operator=(const Tuple& src_tuple) {
	tuple_vals = src_tuple.tuple_vals;
	return *this;
}

Tuple::Tuple(const Schema& src_schema, const BitStream& src_bit_stream) : tuple_schema(src_schema) {
	vector<string> name_list = src_schema.nameList();
	vector<Type> type_list = src_schema.typeList();
	vector<int> length_list = src_schema.lengthList();
	char *slot = new char[BLOCK_SIZE];
	char *pos = src_bit_stream.begin();
	for (int i = 0; i < name_list.size(); i++) {
		memcpy(slot, pos, length_list[i]);
		slot[length_list[i]] = 0;
		switch (type_list[i]) {
		case Type::INT:
			tuple_vals.emplace(name_list[i], *(int*)slot);
			break;
		case Type::DOUBLE:
			tuple_vals.emplace(name_list[i], *(double*)slot);
			break;	
		case Type::STRING:
			tuple_vals.emplace(name_list[i], string(slot));
			break;
		default:
			break;
		}
		pos += length_list[i];
	}
	valid = *pos;
	delete [] slot;
}

BitStream Tuple::toBit() const {
	BitStream ret(tuple_schema.bitLength());
	char *slot = new char[BLOCK_SIZE];
	char *pos = ret.begin();
	vector<string> name_list = tuple_schema.nameList();
	vector<Type> type_list = tuple_schema.typeList();
	vector<int> length_list = tuple_schema.lengthList();
	for (int i = 0; i < name_list.size(); i++) {
		memcpy(pos, tuple_vals.find(name_list[i])->second.toBit().begin(), length_list[i]);
		memcpy(pos+tuple_vals.find(name_list[i])->second.bitLength(), string(255, '\0').c_str(), length_list[i] - tuple_vals.find(name_list[i])->second.bitLength());
		pos += length_list[i];
	}
	*pos = valid;
	return ret;
}

vector<Item> Tuple::valueList(const vector<string>& src_attr_list) const {
	vector<Item> ret;
	for (auto i : src_attr_list) {
		ret.push_back(tuple_vals.find(i)->second);
	}
	return ret;
}

bool Tuple::meet(const Requirement& src_requirement) const {
	bool ret;
	Item target = tuple_vals.find(src_requirement.attribute_name)->second;
	if (target.holdNull()) {
		return true;
	}
	switch (src_requirement.operator_type) {
	case Operator::EQ:
		ret = target == src_requirement.item;
		break;
	case Operator::NE:
		ret = !(target == src_requirement.item);
		break;
	case Operator::LT:
		ret = target < src_requirement.item;
		break;
	case Operator::LE:
		ret = target <= src_requirement.item;
		break;
	case Operator::GT:
		ret = target > src_requirement.item;
		break;
	case Operator::GE:
		ret = target >= src_requirement.item;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

bool Tuple::meetRequirement(const vector<Requirement>& src_requirements) const {
	if (!valid) {
		return 0;
	}
	bool ret = 1;
	for (auto i : src_requirements) {
		if (!meet(i)) {
			ret = 0;
		}
	}
	return ret;
}

void Tuple::_debug_show_info() const {
	cerr << "*************** Showing Tuple Information ****************" << endl;
	tuple_schema._debug_show_info();
	for (auto i : tuple_schema.nameList()) {
		cerr << i << ": " << tuple_vals.find(i)->second.toString() << endl;
	}
	cerr << "**********************************************************" << endl;
}

bool Tuple::_debug_hold_illegal() const {
	auto type_list = tuple_schema.typeList();
	auto name_list = tuple_schema.nameList();
	auto val_list = valueList(name_list);
	for (int i = 0; i < type_list.size(); i++) {
		switch (type_list[i])
		{
		case Type::INT:
			if (val_list[i].int_val == 0) {
				return 1;
			}
			break;
		case Type::DOUBLE:
			if (val_list[i].double_val == 0) {
				return 1;
			}
			break;
		case Type::STRING:
			if (val_list[i].string_val == "") {
				return 1;
			}
			break;
		default:
			break;
		}
	}	
	return 0;
}

void Tuple::makeInvalid() {
	valid = 0;
}

bool Tuple::operator<(const Tuple& src_tuple) const {
	auto first_attr_name = tuple_schema.fisrtAttribute();
	return tuple_vals.find(first_attr_name)->second < src_tuple.tuple_vals.find(first_attr_name)->second;
}