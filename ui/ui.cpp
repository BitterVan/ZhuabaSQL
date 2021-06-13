#include "ui.hpp"
using namespace std;

UI::UI() = default;
UI::~UI() = default;

string UI::fetchStatement() const {
	string ret;
	string temp;
	while (1) {
		cout << ">>> ";
		getline(cin, temp);
		ret += temp;
		if (temp.back() == ';') {
			break;
		} else {
			ret += " ";
		}
	}
	return ret;
}

void UI::plotTable(const vector<Tuple>& src_tuple_list) const{
	for (auto i : src_tuple_list) {
		i._debug_show_info();
	}
}