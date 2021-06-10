#pragma once
#include "basics.hpp"
using namespace std;

class Item {
	private:
		int int_val;
		double double_val;
		string string_val;
		Type item_type;
	public:
		Item(int);
		Item(double);
		Item(const string&);
		Item(const Item&);
		Item() = delete;
		~Item();
		string toString() const;
		bool operator==(const Item&) const;
		bool operator<(const Item&) const;
		bool operator<=(const Item&) const;
		bool operator>=(const Item&) const;
		bool operator>(const Item&) const;
};