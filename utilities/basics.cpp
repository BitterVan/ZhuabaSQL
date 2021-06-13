#include "basics.hpp"
using namespace std;

int str2int(const string& src_string) {
	stringstream temp(src_string);
	int ret;
	temp >> ret;
	return ret;
}

double str2double(const string& src_string) {
	stringstream temp(src_string);
	double ret;
	temp >> ret;
	return ret;
}

