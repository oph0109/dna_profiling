//
// CS 251 project 1 - DNA Profiling
// Owen Hirschey
// netID - ohirs2
// this program parses DNA files
// enter 'help' when the program is running for more information
//

#include "ourvector.h"
#include "util.h"
using namespace std;



int main() {
    ourvector<ourvector<char>> data;
    ourvector<char> dna;
    ourvector<int> processed;
    util f;
    string dbfile;
    string dnafile;
    string input;
    string exportFname;

    cout << "Welcome to the DNA Profiling Application." << endl;
    while (true) {
        cout << "Enter command or # to exit: ";
        getline(cin, input);
        if (input.substr(0, 7) == "load_db") {
            dbfile = input.substr(8);
            data = f.load_db(dbfile);
        } else if (input.substr(0, 8) == "load_dna") {
            dnafile = input.substr(9);
            dna = f.load_dna(dnafile);
        } else if (input.substr(0, 7) == "display") {
            f.display(data, dna, processed);
        } else if (input.substr(0, 7) == "process") {
            processed = f.process(data, dna);
        } else if (input.substr(0, 6) == "search") {
            f.search(data, processed);
        } else if (input.substr(0, 6) == "export") {
            if (input.length() >= 8) {
            exportFname = input.substr(7);
            f.exportFile(data, dna, processed, exportFname);
            } else {
                cout << "Error: enter a filename." << endl;
                cout << "Export usage: export [filename]" << endl;
            }
        } else if (input.substr(0, 4) == "help") {
            cout << endl;
            cout << "load_db [filename] - ";
            cout << "loads database file into the storage" << endl;
            cout << "load_dna [filename] - ";
            cout << "loads dna file into the storage" << endl;
            cout << "process - ";
            cout << "finds the longest repetition of each dna strand" << endl;
            cout << "search - searches all people for matching DNA data" << endl;
            cout << "display - displays the stored data" << endl;
            cout << "export [filename] - exports the stored data to a text file" << endl;
            cout << "# - exits the program" << endl;
        } else if (input.substr(0, 1) == "#") {
            return 0;
        }
        cout << endl;
    }
}
