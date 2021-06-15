#include "../interpreter/interpreter.hpp"
#include "../ui/ui.hpp"
#include <chrono>
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
		auto begin = chrono::system_clock::now();
		if (temp == "quit;") {
			break;
		} else {
			try {
				interpreter.interpret(temp);
				auto end = chrono::system_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(end - begin);
				cout <<  "Finish execution in " 
					<< double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den 
					<< " seconds" << endl;
			} catch (exception& exp) {
				cerr << exp.what() << endl;
			}
		}
	}
}