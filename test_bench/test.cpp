#include <cstdio>
#include <fstream>
using namespace std;

int main() {
    fstream file("test_file");
    file.seekp(0);
    file.write("542342542543253425432552", 20);
}
