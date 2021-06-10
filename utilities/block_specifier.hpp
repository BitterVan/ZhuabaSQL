#pragma once
#include "basics.hpp"
using namespace std;

class BlockSpecifier {
	private:
		string schema_name;
		int page_number;
	public:
		BlockSpecifier(const string&, int);
		BlockSpecifier(const BlockSpecifier&);
		~BlockSpecifier();
};