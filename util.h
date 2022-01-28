//  
//  util.h
//  This declares all of the functions which pull data from the files
//  also includes vector manipulation
//

#include <fstream>
#include <iostream>
#include <string>
#include "ourvector.h"
using namespace std;

class util {
    public:
        ourvector<ourvector<char>> load_db(string fname) { //loads database using filename
            ourvector<ourvector<char>> strArr;
            ourvector<char> temp;
            cout << "Loading database...";
            openFile(fname);
            skipDB(); //skips the 'name,' portion of the db file
            if (in) {
                while (!in.eof()) {
                    temp = pullChars();
                    if (isDNA(temp)) {
                        temp.push_back('!'); //! - indicates that the string is a piece of DNA data
                        strArr.push_back(temp);
                        numStrands++;
                    } else if (isdigit(temp[0])) {
                        temp.push_back('#'); //# - indicates that the string is a number
                        strArr.push_back(temp);
                    } else if (isalpha(temp[0])) {
                        temp.push_back('>'); //> - indicates that the    string is a name
                        strArr.push_back(temp);
                        numPeople++;
                    }
                }
                temp.clear();
                in.close();
                dbStored = true;
            }
            return strArr;
        }

    ourvector<char> load_dna(string fname) {    //stores DNA
        ourvector<char> dna;
        cout << "Loading DNA...";
        openFile(fname);
        if (in) {
            ourvector<char> temp = pullDNA();
            for (char c: temp) {
                dna.push_back(c);
            }in.close();
            dnaStored = true;
        }
        return dna;
    }

    void display(ourvector<ourvector<char>> db, ourvector<char> dna, ourvector<int> proc) {
        if (dbStored) {
            print_db(db);
            cout << endl;
        } else
            cout << "No database loaded." << endl;
        if (dnaStored) {
            cout << "\nDNA loaded: " << endl;
            printVec(dna);
            cout << endl;
        } else
            cout << "\nNo DNA loaded." << endl;

        if (dbProcessed) {
            cout << "\nDNA processed, STR counts: " << endl;
            for (int i = 0; i < numStrands; i++) {
                printvec_db(db[i]);
                cout << ": " << proc[i] << endl;
            }
        } else
            cout << "\nNo DNA has been processed.";

    }

    ourvector<int> process(ourvector<ourvector<char>> db, ourvector<char> dna) {
        ourvector<int> counts;
        if (dbStored && dnaStored) {
            cout << "Processing DNA...";
            ourvector<ourvector<char>> strands;
            ourvector<char> temp;
            ourvector<int> prevCounts;

            for (int i = 0; i < numStrands; i++) { //strand index is same as count
                counts.push_back(0);
                prevCounts.push_back(0);
            }

            strands = moveStrands(db);

            for (int j = 0; j < numStrands; j++) { //algo for checking recurring substrings
                for (int i = 0; i < dna.size(); i++) {
                    if (checkvec(strands[j], dna, i) == 1) {
                        counts[j]++;
                        i += strands[j].size() - 1;
                    } else if (counts[j] > prevCounts[j]) {
                        prevCounts[j] = counts[j];
                        counts[j] = 0;
                    } else
                        counts[j] = 0;

                }
            }
            dbProcessed = true;
            return prevCounts;
        } else if (!dbStored) { //checker to make sure that db and dna are loaded
            cout << "No database loaded.";
        } else if (!dnaStored) {
            cout << "No DNA loaded.";
        }
        return counts;
    }

    int search(ourvector<ourvector<char>> db, ourvector<int> p) {
        ourvector<int> nums;
        int index;
        if (db.size() != 0 && dnaStored && dbProcessed) {
            cout << "Searching database..." << endl;

            for (int i = 0; i < db.size(); i++) {
                if (db[i].back() == '>') {
                    nums.clear();
                    index = i;
                } else if (db[i].back() == '#') {
                    nums.push_back(getInt(db[i]));
                    if (checkvec(nums, p)) {
                        cout << "Found in database!  DNA matches: ";
                        printvec_db(db[index]);
                        return index;
                    }
                }

            }
            cout << "Not found in database.";
        }
        if (db.size() == 0) {
            cout << "No database loaded.";
        } else if (!dnaStored) {
            cout << "No DNA loaded.";
        } else if (!dbProcessed) {
            cout << "No DNA processed.";
        }
        return 0;

    }
            // exportFile functions the same as display() except is exports to a file,
            // and also leaves out a few unnecessary details
    void exportFile(ourvector<ourvector<char>> db, ourvector<char> dna, ourvector<int> proc, string fname) {
        out.open(fname);
        if (!out) {
            out << "output file '" << fname << "' cannot be opened.";
        } else {
            if (db.size() != 0) {
                print_db_e(db);
                out << endl;
            } else
                out << "No database loaded." << endl;
            if (dna.size() != 0) {
                out << "\nDNA loaded: " << endl;
                printVec_e(dna);
                out << endl;
            } else
                out << "\nNo DNA loaded." << endl;

            if (proc.size() != 0) {
                out << "\nDNA processed, STR counts: " << endl;
                for (int i = 0; i < numStrands; i++) {
                    printvec_db_e(db[i]);
                    out << ": " << proc[i] << endl;
                }
            } else
                out << "\nNo DNA has been processed.";
        }

        search_e(db, proc);
        cout << "Exported stored data." << endl;
        out.close();
    }
    
    private:
    ifstream in;
    ofstream out;
    bool dnaStored = false;
    bool dbStored = false;
    bool dbProcessed = false;
    int numStrands;
    int numPeople;

    void openFile(string file) { //utility function which outputs the error message
       in.open(file);
        if (!in) {
            cout << "\nError: unable to open '" << file << "'";
        }
    }

    void skipDB() { //skip first 5 characters to reach DNA data
        if (in) {
            for (int i = 0; i < 5; i++) {
               in.get();
            }
        }
    }

    void print_db(ourvector<ourvector<char>> strArr) { //prints stored database
        cout << "Database loaded: ";
        for (int i = 0; i < strArr.size(); i++) {
            if (strArr[i].back() == '!' && !isspace(strArr[i][0])) {

            } else if (strArr[i].back() == '#') {
                cout << " ";
                printvec_db(strArr[i]);
            } else if (strArr[i].back() == '>') {
                cout << endl;
                printvec_db(strArr[i]);
            }
        }
    }

    void print_db_e(ourvector<ourvector<char>> strArr) {
        out << "Database loaded: ";
        for (int i = 0; i < strArr.size(); i++) {
            if (strArr[i].back() == '!' && !isspace(strArr[i][0])) {

            } else if (strArr[i].back() == '#') {
                out << " ";
                printvec_db_e(strArr[i]);
            } else if (strArr[i].back() == '>') {
                out << endl;
                printvec_db_e(strArr[i]);
            }
        }
    }

    void printVec(ourvector<char> str) { //prints entire vector
        for (int i = 0; i < str.size(); i++) {
            cout << str.at(i);
        }
    }

    void printVec_e(ourvector<char> str) { //outputs entire vector to out
        for (int i = 0; i < str.size(); i++) {
            out << str.at(i);
        }
    }

    void printvec_db(ourvector<char> str) { //does not print identifier
        for (int i = 0; i < str.size() - 1; i++) {
            cout << str[i];
        }
    }

    void printvec_db_e(ourvector<char> str) { //same as printvec_db except it exports to output stream instead of console
        for (int i = 0; i < str.size() - 1; i++) {
            out << str[i];
        }
    }

    ourvector<char> pullChars() { //pulls next comma or whitespace separated string
        char c;
        ourvector<char> temp;
        if (!in) {
            exit(0);
        }
        while (c != ',' && !in.eof() && c != '\n') {
           in.get(c);
            if (c == '\n' || c == ',')
                return temp;
            temp.push_back(c);

        }
        return temp;
    }

    int getInt(ourvector<char> a) { //helper function to get int from char
        string temp;

        for (int i = 0; i < a.size(); i++) {
            if (a[i] != '#') {
                temp.push_back(a[i]);
            }
        }
        return stoi(temp);
    }

    ourvector<char> pullDNA() {
        char c;
        ourvector<char> temp;
        if (!in) {
            exit(0);
        }
        while (c != ',' && !in.eof() && c != '\n') {
           in.get(c);
            if (c == '\n' || c == ',')
                return temp;
            if (!in.fail())
                temp.push_back(c);

        }
        return temp;
    }

    bool isDNA(ourvector<char> str) { //determines whether vec of chars is piece of DNA data or not
        if (isdigit(str[0]))
            return false;

        if (!isalpha(str[0]))
            return false;

        if (str[1] != toupper(str[1]))
            return false;

        return true;
    }

    bool checkvec(ourvector<char> a, ourvector<char> b, int j) { //j is the index which b is starting from
        for (int i = 0; i < a.size(); i++) {
            if (j + i < b.size()) {
                if (a[i] != b[i + j]) {
                    return false;
                }
            } else {
                return false;
            }
        }
        return true;
    }

    bool checkvec(ourvector<int> a, ourvector<int> b) { //checks if 2 integer vectors are the same
        if (a.size() != b.size()) {
            return false;
        }
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    ourvector<ourvector<char>> moveStrands(ourvector<ourvector<char>> a) {
        ourvector<ourvector<char>> b;
        ourvector<char> temp;
        for(int i = 0; i < numStrands;i++) {
            b.push_back(temp);
        }
        for (int i = 0; i < numStrands; i++) {
            for (int j = 0; j < a[i].size(); j++) {
                if (a[i][j] != '!')
                    b[i].push_back(a[i][j]);
            }
        }
        temp.clear();
        return b;
    }

    int search_e(ourvector<ourvector<char>> db, ourvector<int> p) {
        ourvector<int> nums;
        int index;
        if (db.size() != 0 && dnaStored && dbProcessed) {

            for (int i = 0; i < db.size(); i++) {
                if (db[i].back() == '>') {
                    nums.clear();
                    index = i;
                } else if (db[i].back() == '#') {
                    nums.push_back(getInt(db[i]));
                    if (checkvec(nums, p)) {
                        out << "Found in database!  DNA matches: ";
                        printvec_db_e(db[index]);
                        return index;
                    }
                }

            }
            out << "Not found in database.";
        }
        return 0;
    }
};