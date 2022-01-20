#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main(){
    ofstream fout;

    ostringstream sout;

    string filename;
    string x, y, v;

    cout << "9x9 Sudoku File Generator\n File name: "; 
    cin >> filename;

    sout << filename << ".txt";
    filename = sout.str();
    
    char* fname = const_cast<char*>(filename.c_str());

    cout << "\nType STOP to finish.\n";

    fout.open(fname);

    for(int count = 0; count < 81; count++){
        cout << "\nEnter coordinates of cell:\n x = ";
        cin >> x;
        if(x == "STOP" || x == "stop") break;
        cout << " y = ";
        cin >> y;
        if(y == "STOP" || x == "stop") break;
        cout << "Enter value of cell: ";
        cin >> v;
        if(v == "STOP" || x == "stop") break;

        fout << x << ' ' << y << ' ' << v << endl;
    }

    fout.close();

    cout << "\nCreated file " << fname << '.' << endl;

    return 0;
}