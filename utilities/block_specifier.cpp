#include "block_specifier.hpp"
using namespace std;

BlockSpecifier::BlockSpecifier(const string& src_name, int src_number) : schema_name(src_name), page_number(src_number) {}

BlockSpecifier::BlockSpecifier(const BlockSpecifier&) = default;

BlockSpecifier::~BlockSpecifier() = default;

string BlockSpecifier::schemaName() const {
	return schema_name;
}

int BlockSpecifier::pageNumber() const {
	return page_number;
}