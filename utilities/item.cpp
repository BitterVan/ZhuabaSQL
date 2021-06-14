#include "item.hpp"
using namespace std;

Item::Item(int src_int_val) : int_val(src_int_val) {
	item_type = Type::INT;
}

Item::Item(double src_double_val) : double_val(src_double_val) {
	item_type = Type::DOUBLE;
}

Item::Item(const string& src_string_val) : string_val(src_string_val) {
	item_type = Type::STRING;
}

Item::Item(const Item&) = default;

Item::Item() = default;

Item::~Item() = default;

string Item::toString() const {
	if (holdNull()) {
		return "null";
	}
	string ret;
	switch (item_type) {
	case Type::INT:
		ret = to_string(int_val);
		break;
	case Type::DOUBLE:
		ret = to_string(double_val);
		break;	
	case Type::STRING:
		ret = "'" + string_val + "'";
		break;
	default:
		break;
	}
	return ret;
}

bool Item::operator==(const Item& src_item) const {
	bool ret = 0;
	switch (item_type) {
	case Type::INT:
		ret = int_val == src_item.int_val;
		break;
	case Type::DOUBLE:
		ret = double_val == src_item.double_val;
		break;
	case Type::STRING:
		ret = string_val == src_item.string_val;
		break;
	default:
		break;
	}
	return ret;
}

bool Item::operator<(const Item& src_item) const {
	bool ret = 0;
	switch (item_type) {
	case Type::INT:
		ret = int_val < src_item.int_val;
		break;
	case Type::DOUBLE:
		ret = double_val < src_item.double_val;
		break;
	case Type::STRING:
		ret = string_val < src_item.string_val;
		break;
	default:
		break;
	}
	return ret;
}

bool Item::operator<=(const Item& src_item) const {
	bool ret = 0;
	switch (item_type) {
	case Type::INT:
		ret = int_val <= src_item.int_val;
		break;
	case Type::DOUBLE:
		ret = double_val <= src_item.double_val;
		break;
	case Type::STRING:
		ret = string_val <= src_item.string_val;
		break;
	default:
		break;
	}
	return ret;
}

bool Item::operator>=(const Item& src_item) const {
	bool ret = 0;
	switch (item_type) {
	case Type::INT:
		ret = int_val >= src_item.int_val;
		break;
	case Type::DOUBLE:
		ret = double_val >= src_item.double_val;
		break;
	case Type::STRING:
		ret = string_val >= src_item.string_val;
		break;
	default:
		break;
	}
	return ret;
}

bool Item::operator>(const Item& src_item) const {
	bool ret = 0;
	switch (item_type) {
	case Type::INT:
		ret = int_val > src_item.int_val;
		break;
	case Type::DOUBLE:
		ret = double_val > src_item.double_val;
		break;
	case Type::STRING:
		ret = string_val > src_item.string_val;
		break;
	default:
		break;
	}
	return ret;
}

int Item::bitLength() const {
	int ret = 0;
	switch (item_type)
	{
	case Type::INT:
		ret = sizeof(int);
		break;
	case Type::DOUBLE:
		ret = sizeof(double);
		break;
	case Type::STRING:
		ret = string_val.length();
		break;
	default:
		break;
	}
	return ret;
}

BitStream Item::toBit() const {
	BitStream ret(bitLength());
	switch (item_type)
	{
	case Type::INT:
		memcpy(ret.begin(), (char*)&int_val, sizeof(int));
		break;
	case Type::DOUBLE:
		memcpy(ret.begin(), (char*)&double_val, sizeof(double));
		break;
	case Type::STRING:
		memcpy(ret.begin(), string_val.c_str(), string_val.length());
		break;
	default:
		break;
	}
	return ret;
}

bool Item::holdNull() const {
	bool ret;
	switch (item_type)
	{
	case Type::INT:
		ret = int_val == INVALID_INT;
		break;
	case Type::DOUBLE:
		ret = double_val == INVALID_DOUBLE;
		break;
	case Type::STRING:
		ret = string_val == INVALID_STRING;
		break;
	default:
		break;
	}
	return ret;
}