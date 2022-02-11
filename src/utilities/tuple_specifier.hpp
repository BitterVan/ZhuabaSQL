#pragma once
#include "basics.hpp"
using namespace std;

struct TupleSpecifier {
	string file_name;
	int page_number;
	int tuple_number;

	TupleSpecifier(const string&, int, int);
	TupleSpecifier(const TupleSpecifier&);
	TupleSpecifier();
	~TupleSpecifier();
};