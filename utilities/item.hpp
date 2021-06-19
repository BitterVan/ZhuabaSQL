#pragma once
#include "basics.hpp"
#include "bit_stream.hpp"
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
		Item();
		~Item();
		string toString() const;
		int bitLength() const;
		bool operator==(const Item&) const;
		bool operator<(const Item&) const;
		bool operator<=(const Item&) const;
		bool operator>=(const Item&) const;
		bool operator>(const Item&) const;
		BitStream toBit() const;
		bool holdNull() const;

		friend class Tuple;
		friend class Block;
		friend class hash<Item>;
};

namespace std {
	template<>
		class hash<Item> {
			public:
			size_t operator() (const Item& src_item) const {
				switch (src_item.item_type)
				{
				case Type::INT:
					return hash<int>()(src_item.int_val);
				case Type::DOUBLE:
					return hash<double>()(src_item.double_val);
				case Type::STRING:
					return hash<string>()(src_item.string_val);
				default:
					return 0;
					break;
				}
			}
		};
};