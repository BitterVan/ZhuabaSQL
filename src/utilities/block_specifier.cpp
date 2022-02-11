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

bool BlockSpecifier::operator==(const BlockSpecifier& src_specifier) const {
	return (schema_name == src_specifier.schema_name) && (page_number == src_specifier.page_number);
}

// size_t block_spec_hash(const BlockSpecifier& src_specifier) {
// 	return hash<string>()(src_specifier.schemaName()) ^ hash<int>()(src_specifier.pageNumber());
// }