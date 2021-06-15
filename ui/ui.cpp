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

void UI::plotTable(const vector<string>& src_name_list, const vector<Type>& src_type_list, const vector<int>& src_length_list, const vector<Tuple>& src_tuple_list, const vector<string>& src_attr_names) const{
	// cout << "Select " << src_tuple_list.size() << " results" << endl;
	switch (src_tuple_list.size())
	{
	case 0:
		return;
		cout << "Empty set" << endl;
		break;
	case 1:
		// cout << "1 record selected" << endl;
		break;
	default:
		// cout << src_tuple_list.size() << " records selected" << endl;
		break;
	}
	vector<string> titles;
	vector<string> new_titles;
	for (int i = 0; i < src_name_list.size(); i++) {
		string temp = src_name_list[i];
		switch (src_type_list[i])
		{
		case Type::INT:
			temp += "(int)";
			break;
		case Type::DOUBLE:
			temp += "(float)";
			break;
		case Type::STRING:
			temp += "(char(" + to_string(src_length_list[i]) + "))";
			break;
		default:
			break;
		}
		new_titles.push_back(temp);
	}
	vector<vector<string>> to_print_table;
	to_print_table.push_back(new_titles);
	for (auto i : src_tuple_list) {
		vector<string> temp;
		auto item_list = i.valueList(src_attr_names);
		for (auto j : item_list) {
			temp.push_back(j.toString());
		}
		to_print_table.push_back(temp);
	}


	vector<int> max_lengths(src_attr_names.size(), 0);
	for (auto i : to_print_table) {
		for (auto j = i.begin(); j != i.end(); j++) {
			max_lengths[j - i.begin()] = max(max_lengths[j - i.begin()], int(j->length()));
		}
	}

	vector<vector<string>> new_print;

	for (auto i : to_print_table) {
		vector<string> new_tuple;
		for (auto j = i.begin(); j != i.end(); j++) {
			*j = "|" + *j + string(max_lengths[j - i.begin()] - j->length() + 1, ' ');
			new_tuple.push_back(*j);
		}
		new_print.push_back(new_tuple);
	}

	for (auto i : new_print) {
		for (int j = 0; j < max_lengths.size(); j++) {
			cout << "+" << string(max_lengths[j]+1, '-');
		}
		cout << "+" << endl;;
		for (auto j : i) {
			cout << j;
		}
		cout << "|" << endl;;
	}
	for (int j = 0; j < max_lengths.size(); j++) {
		cout << "+" << string(max_lengths[j]+1, '-');
	}
	cout << "+" << endl;;
	switch (src_tuple_list.size())
	{
	case 0:
		cout << "Empty set" << endl;
		break;
	case 1:
		cout << "1 record selected" << endl;
		break;
	default:
		cout << src_tuple_list.size() << " records selected" << endl;
		break;
	}
}