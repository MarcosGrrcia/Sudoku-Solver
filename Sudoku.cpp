#include <sys/time.h>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class omega {
        public:
                omega();
                float get_sec();

        private:
                long Tinit;
                long Tmark;
};

omega::omega() {
        struct timeval T;
        gettimeofday(&T, NULL);

        Tinit = 1000000*T.tv_sec + T.tv_usec;
}

float omega::get_sec() {
        struct timeval T;
        gettimeofday(&T, NULL);

        Tmark = 1000000*T.tv_sec + T.tv_usec;

        return (float)(Tmark-Tinit)/1000000.0;
}

string elapsed(float dur, int i = 0) {
        const char *units[] = { "sec", "msec", "usec" };	// if base case return solution
        
	if(dur >= 0.1){
                ostringstream oss;
                oss <<  endl << "Time: " << fixed << setprecision(2) << dur << ' ' <<units[i]
                        << " (" << setprecision(6) << dur << ' ' << units[i] <<")" << endl;
                return oss.str();
        }

        return elapsed(dur * 1000, i + 1);

}

class sudoku {
        public:
                sudoku();

                void solve();

                void read(const char *);
                void write(const char *);
                void write(const char *, const char *);

        private:
                bool solve(vector<int>& , unsigned int c);

                vector<int>valid_values(int a,int b){
                        vector<int>valid;
                        for(int v = 1; v < 10; v++){
                                game[a][b] = v;
                                if(check_row(a, v) && check_col(b, v) && check_square(a, b, v))
                                        valid.push_back(v);
                        }
                        game[a][b] = 0;
                        return valid;
                }

                bool error_check_value(bool);
                bool error_check_uniqueness();
                bool check_row(int, int);
                bool check_col(int, int);
                bool check_square(int, int, int);

                void display();

                int game[9][9];
};

sudoku::sudoku() {
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++)
                        game[i][j] = 0;
        }
}

void sudoku::solve() {
        cout << "SOLVE\n";
        int t = 0;

        vector<int>cells;
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++){
                        t = game[i][j];
                        if(t == 0)
                                cells.push_back(i * 9 + j);
                }
        }

        if (solve(cells, 0))		//call recursive solve function
                display();

        if(error_check_value(false) && error_check_uniqueness())		//error check data values and uniqueness exit if errors detected
                exit(0);
}

bool sudoku::error_check_value(bool read){
        bool err = false;
        int t = 0;
        for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                        t = game[i][j];
                        if(read == true){
                                if(t < 0 || t > 9){
                                        cerr << "cell " << i << ' ' << j << ": out-of-bounds data value " << t << endl;
                                        err = true;
                                }
                        }
                        if(read == false){
                                if(t < 1 || t > 9){
                                        cerr << "cell " << i << ' ' << j << ": out-of-bounds data value " << t << endl;
                                        err = true;
                                }
                        }
                }
        }
        return err;
}

bool sudoku::check_col(int c, int v){
        int count = 0;
        for(int i = 0; i < 9; i++){
                if(game[i][c] == v)
                        count++;
        }
        if(count > 1)
                return false;

        return true;
}

bool sudoku::check_row(int r, int v){
        int cnt = 0;
        for(int j = 0; j < 9; j++){
                if(game[r][j] == v)
                        cnt++;
        }
        if(cnt > 1)
                return false;

        return true;
}

bool sudoku::check_square(int r, int c, int v){
        int cnt = 0;
        r = (r/3)*3;
        c = (c/3)*3;

        for(int i = r; i < r + 3; i++){
                for(int j = c; j < c + 3; j++){
                        if(game[i][j] == v)
                                cnt++;
                }
        }
        if(cnt > 1)
                return false;

        return true;
}

bool sudoku::error_check_uniqueness(){
        bool err = false;
        for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                        if(game[i][j] != 0){
                                if(!(check_row(i,game[i][j]) && check_col(j, game[i][j]) && check_square(i,j,game[i][j]))){
                                        cerr << "cell " << i << " " << j <<": non-unique value " << game[i][j] << endl;
                                        err = true;
                                }
                        }
                }
        }
        return err;
}


void sudoku::read(const char *fname) {
        int line = 1;
        bool error = false;

        cout << "READ\n";

        ifstream fin(fname);

        int i, j, v;

        while (fin >> i >> j >> v) {
                //      error check grid indices
                if( (i < 0 || i > 8) || (j < 0 || j > 8) ){
                        cerr << "line " << line << ": " << i << ' '<< j << ' ' << v << " out-of-bounds grid index\n";
                        error = true;
                }
                line++;
                game[i][j] = v;
        }

        fin.close();

        if(error == true){		 //exit if bad grid indices

                exit(0);
	}

	display();

        if(error_check_value(true)){		// error check data values
                error = true;
	}

        if(error_check_uniqueness()){		// error check uniqueness
                error = true;
	}

        if(error == true){			// exit if errors detected
                exit(0);
	}
}

void sudoku::write(const char *fname) {
        ofstream fout(fname);

        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        if (0 < game[i][j]) {
                                fout << i << " "
                                        << j << " "
                                        << game[i][j] << "\n";
                        }
                }
        }

        fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
        int N1 = strlen(fname);
        int N2 = strlen(addon);

        char *n_fname = new char[N1+N2+2];

        //strip .txt suffix, then concatenate _addon.txt
        strncpy(n_fname, fname, N1-4);
        strcpy(n_fname+N1-4, "_");
        strcpy(n_fname+N1-3, addon);
        strcpy(n_fname+N1-3+N2, ".txt");

        write(n_fname);

        delete [] n_fname;
}

void sudoku::display() {
        cout << "| --------------------------- |\n";
        for (int i = 0; i < 9; i++) {
                for (int j=0; j < 9; j++) {
                        if (j == 0){
                                cout << "| " << game[i][j];
			}
                        else if (j%3 == 0){
                                cout << " | " << game[i][j];
                        }
			else{
                                cout << "  " << game[i][j];
                	}
		}
                cout << " |\n";
                if (i % 3 == 2)
                        cout << "| --------------------------- |\n";
        }
}

bool sudoku::solve(vector<int>& cells, unsigned int c) {

        // if solution found, return solution-found
        if(c == cells.size())
                return true;

        unsigned int lowest_vv = 10;
        int lowest_vv_i = 0;

        for(unsigned int index = c; index < cells.size(); index++){
                int i = cells[index]/9;
                int j = cells[index]%9;

                unsigned int temp = valid_values(i,j).size();
                if(temp == 0)
                        return false;

                if(temp < lowest_vv){
                        lowest_vv = temp;
                        lowest_vv_i = index;
                }
        }

        swap(cells[c], cells[lowest_vv_i]);
        //set cell index (i,j)
        int i = cells[c]/9;
        int j = cells[c]%9;

        vector<int>values = valid_values(i,j);		// determine valid values

        if(values.size() == 0){			// if no valid values left return road-to-nowhere
                return false;
	}

        for(unsigned int index = 0; index < values.size(); index++){	// iterate thru valid values game[i][j] = next value
                game[i][j] = values[index];

                if(solve(cells, c + 1) == true){		// if solve(arguments) == solution-found return solution-found
                        return true;
		}

        }

        game[i][j] = 0;			// reset: game[i][j] = 0 return road-to-nowhere
        return false;
}

void generate(){                //creates readable sudoku file
    ofstream fout;

    ostringstream sout;

    string filename;
    string x, y, v;

    cout << "\n9x9 Sudoku File Generator\n File name: "; 
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
}

int main(int argc, char *argv[]) {
        
        srand(time(NULL));
        char* Fname;

        if((strcmp(argv[1], "-s") != 0) || (argc > 3) || (argc < 2)){
                cerr << "usage: Sudoku -s [Sudoku .txt file]\n";
                exit(0);
        }
        
        if(argc == 3){
               if(strstr(argv[argc-1], ".txt") == NULL){
                        cerr << "usage: Sudoku -s [game .txt file]\n";
                        exit(0);
                }
                Fname = argv[2];
        }
        else{
                char input;
                cout << "Create custom Sudoku game? (y or n) ";
                cin >> input;
                if(input == 'y' || input == 'Y'){
                        generate();
                        return 0;
                }else if(input == 'n' || input == 'N'){
                        cerr << "\nusage: Sudoku -s [game .txt file]\n";
                        exit(0);
                }else{
                        cerr << "\nCould not understand response.\n";
                        exit(0);
                }
        }

        omega timer;
        sudoku sudoku_game;

        float T0 = 0.0;
        float T1 = 0.0;

        if (strcmp(argv[1], "-s") == 0) {
                sudoku_game.read(Fname);

                T0 = timer.get_sec();
                sudoku_game.solve();
                T1 = timer.get_sec();

                sudoku_game.write(Fname, "solved");
        }

        string elapsed_time = elapsed(T1-T0);		// print string elapsed_time using two decimal places

        cout << elapsed_time;		// print T1-T0 using six decimal places for comparison

        return 0;
}

