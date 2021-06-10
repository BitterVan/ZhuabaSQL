#include "../utilities/item.hpp"
using namespace std;

int main() {
	unordered_map<string, Item> test_map;
	test_map.emplace("hello", 2);
	// test_map["hello"];
	test_map.find("hello")->first;
	// test_map["world"] = 1;
}