#pragma once
#include "item.hpp"
#include "bit_stream.hpp"
#include "requirement.hpp"
#include "schema.hpp"
#include <utility>
using namespace std;

class Tuple {
	private:
		const Schema& tuple_schema;
		unordered_map<string, Item> tuple_vals;
		bool meet(const Requirement&) const;
		char valid;

	public:
		Tuple(const Tuple&);
		Tuple() = delete;
		~Tuple();
		Tuple(const Schema&, const vector<Item>&);
		Tuple(const Schema&, const BitStream&);
		Tuple& operator=(const Tuple&);
		BitStream toBit() const;
		vector<Item> valueList(const vector<string>&) const;
		bool meetRequirement(const vector<Requirement>&) const;
		void makeInvalid();
		void _debug_show_info() const;
		bool _debug_hold_illegal() const;
};