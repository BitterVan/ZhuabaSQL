#include "../interpreter/interpreter.hpp"
#include "../ui/ui.hpp"
#include <chrono>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	BufferPool buffer;
	CatalogManager catalog(buffer);
	RecordManager record(buffer);
	IndexManager index(buffer, catalog, record);
	UI ui;
	API api(catalog, index, record, ui);
	Interpreter interpreter(&api);
	string temp;
	while (1) {
		temp = api.fetchStatement();
		auto begin = chrono::system_clock::now();
		if (temp == "quit;") {
			break;
		} else if (temp.find("execfile") == temp.npos) {
			try {
				interpreter.interpret(temp);
			} catch (exception& exp) {
				cerr << exp.what() << endl;
				continue;
			}
		} else {
			string word;
			stringstream temp_stream(temp);
			temp_stream >> word;
			temp_stream >> word;
			auto inst_list = api.fetchFile(string(word, 0, word.length()-1));
			// cerr << "fetch list" << endl;
			for (auto i : inst_list) {
				// auto start = chrono::system_clock::now();
				if (i == "quit;") {
					return 0;
				}
				try {
					interpreter.interpret(i);
				} catch (exception& exp) {
					cerr << exp.what() << endl;
					continue;
				}
			}
		}
		auto end = chrono::system_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - begin);
		cout <<  "Finish execution in " 
			<< double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den 
			<< " seconds (include print time)" << endl;
		cout << endl;
	}
}