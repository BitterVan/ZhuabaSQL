#include "tuple_specifier.hpp"
using namespace std;

TupleSpecifier::TupleSpecifier(const string& src_file_name, int src_page_number, int src_tuple_number):
	file_name(src_file_name), page_number(src_page_number), tuple_number(src_tuple_number) {}

TupleSpecifier::TupleSpecifier(const TupleSpecifier&) = default;

TupleSpecifier::TupleSpecifier() = default;

TupleSpecifier::~TupleSpecifier() = default;