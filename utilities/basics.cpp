#include "basics.hpp"
using namespace std;

int str2int(const string& src_string) {
	if (src_string == "null") {
		return INVALID_INT;
	}
	stringstream temp(src_string);
	int ret;
	temp >> ret;
	return ret;
}

double str2double(const string& src_string) {
	if (src_string == "null") {
		return INVALID_DOUBLE;
	}
	stringstream temp(src_string);
	double ret;
	temp >> ret;
	return ret;
}

string str2str(const string& src_string) {
	if (src_string == "null") {
		return INVALID_STRING;
	} else if (src_string[0] == '\'') {
		return src_string;
	} else {
		return "  ";
	}
}