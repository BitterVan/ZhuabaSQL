#include "interpreter/interpreter.hpp"
using namespace std;

int main() {
	BufferPool buffer;
	CatalogManager catalog(buffer);
	RecordManager record(buffer);
	API api(catalog, record);
	Interpreter interpreter(&api);
	string temp;
	while (1) {
		getline(cin, temp);
		if (temp == "quit;") {
			break;
		} else {
			interpreter.interpret(temp);
		}
	}
}