#include "interpreter/interpreter.hpp"
#include "ui/ui.hpp"
using namespace std;

int main() {
	BufferPool buffer;
	CatalogManager catalog(buffer);
	RecordManager record(buffer);
	UI ui;
	API api(catalog, record, ui);
	Interpreter interpreter(&api);
	string temp;
	while (1) {
		temp = api.fetchStatement();
		if (temp == "quit;") {
			break;
		} else {
			try {
				interpreter.interpret(temp);
			} catch (exception& exp) {
				cerr << exp.what() << endl;
			}
		}
	}
}