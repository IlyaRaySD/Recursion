// connecting libraries
#include <iostream>
#include "Windows.h"
#include <WinUser.h>
#include <fstream>
#include <stdio.h>
using namespace std;

// variable for use in Sleep() functions
double TIME = 200;

// array of possible knight movements
int moves[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

// number of solutions to the queen puzzle
int num_of_sol = 0;

// console cleaning function
void clear() { system("cls"); }

// function to print array
void print(int** array, int m, int n, char zero) {
	string space = "  ";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (array[j][i] != 0) cout << space << array[j][i];
			else cout << space << zero;
		}
		cout << endl;
	}
	cout << endl;
}

// instruction output function
int opt_catch() {
	int time = 10; // sleep time
	string option = "\nВыберите интересующий алгоритм:";
	for (int i = 0; i < option.length(); i++) {
		Sleep(time);
		cout << option[i];
	}
	cout << endl;

	string instr = "1 - Последовательность Фибоначи\n2 - Задача о ходе коня\n3 - Задача о ходе коня(наглядно)(запускать только в первую очередь!)\n4 - Поиск максимума, разделяй и властвуй\n5 - Ханойские башни\n6 - Ханойские башни(наглядно)\n7 - Задача о ферзях\n8 - Задача о ферзях(наглядно)(запускать только в первую очередь!)\n0 - Выход из программы\n\nВводите номер: ";
	time = 0; // sleep time
	for (int i = 0; i < instr.length(); i++) {
		Sleep(time);
		cout << instr[i];
	}
	int choose;
	cin >> choose;
	return choose;
}

// function to place the cursor in the console
void set_cons(int x, int y, int space) {
	COORD position = { (space + 1) * x, y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);
}

// function to display a symbol in a specific place in the console
template<typename T>
void anim(int x, int y, int space, T num, clock_t start_time) {
	clock_t end_time = clock();
	double search_time = (double)((end_time - start_time)/1000);
	if (search_time > 15) // acceleration start point
		TIME /=  1.2;
	if (search_time > 45) // when enough time has passed the function stops working
		return;
	else {
		Sleep(TIME);
		set_cons(x, y, space);
		cout << "\b";
		Sleep(TIME);
		cout << num << " ";
		Sleep(TIME);
	}
}

// this function works like the anim() function but without acceleration
template<typename T>
void anim_slow(int x, int y, T num) {
	Sleep(TIME/5);
	set_cons(x, y, 2);
	cout << "\b";
	Sleep(TIME/5);
	cout << num << " ";
	Sleep(TIME/5);
}

// this function works like the anim() function but with maximum speed
template<typename T>
void anim_fast(int x, int y,int space, T num) {
	Sleep(0);
	set_cons(x, y, space);
	cout << "\b";
	Sleep(0);
	cout << num << " ";
	Sleep(0);
}

// a function that shows the final result of the knight's move algorithm
void end_horse(int** field, int size, int space) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // variable holding console parameters
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			SetConsoleTextAttribute(console, field[i][j] % 14 + 1); // changing text color
			cout << field[i][j] << " ";
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing text color to white
		}
	}
}

// a function that shows the final result of the queen puzzle algorithm
void end_queen(int** field, int size, int space) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			if (field[i][j] == -1) {
				SetConsoleTextAttribute(console, 11); // changing text color to mint
				cout << 'l' << " ";
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing text color to white
			}
			else cout << 0 << " ";
		}
	}
}

// knight walking algorithm
bool horse_move_an(int** field, int c, int x_h, int y_h, int size, clock_t time) {
	clock_t start_time = clock();
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		// selection of a possible knight move
		x2 = x_h + moves[k][0]; 
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) { // checking for out of bounds
			if (field[x2][y2] == 0) { // checking that the square has not yet been visited
				field[x2][y2] = c;
				SetConsoleTextAttribute(console, c%14 + 1); // changing text color
				anim(x2 + 1, y2 + 1, size / 10 + 2, c, time);
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing text color to white
				if (c < size * size and horse_move_an(field, c + 1, x2, y2, size, time) == 0) { // checking the impossibility of the next move
					field[x2][y2] = 0;
					anim(x2 + 1, y2 + 1, size / 10 + 2, 0, time);
				}
				else { // triggers when the knight has finished traversing the entire field
					end_horse(field, size, size / 10 + 2);
					end = 1;
				}

			}
		}
	}
	return end;
}

// the function is called when choosing a visual algorithm for the knight's move
void horse_turn_an() {
	char space = ' ';
	int n; // field dimension
	clear(); // cleaning the console for correct operation of the function
	cout << "Введите размерность поля (желательно <6): ";
	cin >> n;

	int** field; 
	field = new int* [n];
	for (int i = 0; i < n; i++)
		field[i] = new int[n];

	int space_w = n / 10 + 2; // amount of space between elements
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			field[i][j] = 0;
			cout << string(space_w, space) << field[i][j];
		}
		cout << endl;
	}

	int x_h = 0, y_h = 0; // initial coordinates of the knight
	cout << "Введите координаты коня: \nx = ";
	cin >> x_h;
	cout << "y = ";
	cin >> y_h;
	cout << "Пожалуйста, подождите, это может занять какое-то время";

	clock_t start_time = clock(); // counting processor ticks at the beginning of the algorithm

	anim(x_h, y_h, space_w, 1, start_time); // the knight takes its initial position

	field[x_h - 1][y_h - 1] = 1; 

	if (horse_move_an(field, 2, x_h - 1, y_h - 1, n, 0)) { // checking the possibility of a solution
		cout << endl;
		set_cons(0, n + 6, 1);
		cout << "Конь смог.\n\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
	}
	else {
		set_cons(0, n + 6, 1);
		cout << "\nКонь не смог.\n\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
	}
	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
	
}

// knight walking algorithm without anim() function
bool horse_move(int** field, int c, int x_h, int y_h, int size, ofstream &file) {
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		// selection of a possible knight move
		x2 = x_h + moves[k][0];
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) { // checking for out of bounds
			if (field[x2][y2] == 0) { // checking that the square has not yet been visited
				field[x2][y2] = c;
				file << c << " ход: " << x2 + 1 << ", " << y2 + 1 << endl; // writing the movement of a knight to a file
				if (c < size * size and horse_move(field, c + 1, x2, y2, size, file) == 0) { // checking the impossibility of the next move
					field[x2][y2] = 0;
					file << "Отмена хода " << c << endl; // writinging a move undo to a file
				}
				else // triggers when the knight has finished traversing the entire field
					end = 1;
			}
		}
	}
	return end;
}

// the function is called when selecting the knight's move algorithm
void horse_turn() {
	int n; // field dimension
	cout << "\nВведите размерность поля: ";
	cin >> n;

	int** field;
	field = new int* [n];
	for (int i = 0; i < n; i++)
		field[i] = new int[n];

	for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				field[i][j] = 0;
	}

	int x_h = 0, y_h = 0;
	cout << "\nВведите координаты коня: \nx = ";
	cin >> x_h;
	cout << "y = ";
	cin >> y_h;
	ofstream file; // file writing the sequence of knight moves
	file.open("solution_knight.txt");
	field[x_h-1][y_h-1] = 1;
	file << "Конь начинает движение с " << x_h << ", " << y_h << endl;
	file << endl << "Ходы коня:\n";
	if (horse_move(field, 2, x_h-1, y_h-1, n, file)) { // checking the possibility of a solution
		cout << endl;
		print(field, n, n, 0);
	}
	else
		cout << "\nКонь не смог." << endl;

	
	file.close();
	system("notepad solution_knight.txt"); // opening a file after the algorithm completes
	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
}

// function for counting the element of the Fibonacci sequence
int fibonachi(int num, ofstream& file) {
	file << "\nПопытка найти " << num << "-й элемент\n"; // writing an attempt to find a sequence element
	if (num == 1 or num == 2)
		return 1;
	if (num < 1)
		return 0;
	else
		return (fibonachi(num - 1, file) + fibonachi(num - 2, file));
}

// the function is called when selecting the Fibonacci sequence
void fibon_one_el() {
	int n; // sequence element number
	cout << "Введите порядковый номер элемнта последовательности : ";
	cin >> n;
	ofstream file; // file to write
	file.open("fibon_steps.txt");
	printf("\n%d-й элемент: %u", n, fibonachi(n, file));
	file.close();
	system("notepad fibon_steps.txt"); // opening file
	cout << endl;
}

// function to check the possibility of placing a queen on a square
bool is_safe(char** desk, int row, int col, int n) {
	// check for two queens in one column
	for (int i = 0; i < row; i++) {
		if (desk[i][col] == 'l') {
			return 0;
		}
	}
	// check for two queens on the same diagonal '\'
	for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
		if (desk[i][j] == 'l') {
			return 0;
		}
	}
	// check for two queens on the same diagonal '/'
	for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
		if (desk[i][j] == 'l') {
			return 0;
		}
	}
	return 1;
}

// the function writes the resulting solution to a file
void print_sol(char** desk, int n, ofstream& file) {
	file << "Solution " << num_of_sol << ":\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			file << desk[i][j] << ' ';
		}
		file << endl;
	}
	file << endl;
}

// try to place a queen
void try_queen(char** desk, int row, int n, ofstream& file) {
	if (row == n) { // if all queens are placed
		num_of_sol++;
		print_sol(desk, n, file);
		return;
	}
	for (int i = 0; i < n; i++) {
		if (is_safe(desk, row, i, n)) { // if the queen can be placed
			desk[row][i] = 'l';
			try_queen(desk, row + 1, n, file); // attempt to place the next queen
			desk[row][i] = '0'; // backtrack to display all solutions
		}
	}
}

// the function is called when selecting the queen puzzle
void queen_quest() {
	int n; // number of queens
	cout << "Введите количество ферзей: ";
	cin >> n;
	cout << endl << endl;
	ofstream file;
	file.open("solutions_queen.txt"); // file for writing all solutions to the queens puzzle
	char** desk = new char* [n];
	for (int i = 0; i < n; i++) desk[i] = new char[n];
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) desk[i][j] = '0';
	num_of_sol = 0;
	try_queen(desk, 0, n, file);
	file << "Ферзи отмечены как 'l'";
	file.close();
	cout << "Количество решений: " << num_of_sol << endl << endl;
	system("notepad solutions_queen.txt"); // opening file
	for (int i = 0; i < n; i++)
		delete[] desk[i];
	delete[] desk;
}

// the function sets the queen on the field array and on the screen, as well as its trajectory
void queen_stand_an(int** field, int x, int y, int n, clock_t time) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < n; i++) {
		++field[i][y]; // horizontal path
		anim_fast(i+1, y + 1, n/10+2, field[i][y]);
		++field[x][i]; // vertical path
		anim_fast(x+1, i+1, n/10+2, field[i][y]);
		int diag = y - x + i; // dioganal '\'
		if (diag >= 0 and diag < n) {
			++field[i][diag]; // diagonal path '\'
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
		diag = y + x - i; // dioganal '/'
		if (diag >= 0 and diag < n) {
			++field[i][diag]; // diagonal path '/'
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
	}
	field[x][y] = -1; // set queen
	SetConsoleTextAttribute(console, 11); // changing text color to mint
	anim(x+1, y+1, n/10+2, 'l', time);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // changing text color to white
}

// the function removes the queen on the array of fields and on the screen, as well as its trajectory
void queen_del_an(int** field, int x, int y, int n, clock_t time) {
	for (int i = 0; i < n; i++) {
		--field[i][y]; // horizontal path
		anim_fast(i+1, y+1, n/10+2, field[i][y]);
		--field[x][i]; // vertical path
		anim_fast(x+1, i+1, n/10+2, field[x][i]);
		int diag = y - x + i; // dioganal '\'
		if (diag >= 0 and diag < n) {
			--field[i][diag]; // diagonal path '\'
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
		diag = y + x - i; // dioganal '/'
		if (diag >= 0 and diag < n) {
			--field[i][diag]; // diagonal path '/'
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
	}
	field[x][y] = 0; // remove queen
	anim(x+1, y+1, n/10+2, 0, time);
}

// queen puzzle algorithm
bool queen_an(int** field, int n, int i, clock_t time) {
	bool res = 0;
	for (int j = 0; j < n; j++) {
		if (field[i][j] == 0) { // checking the possibility of placing a queen
			queen_stand_an(field, i, j, n, time);
			if (i == n - 1) res = 1; // if all queens are placed
			else {
				if (!(res = queen_an(field, n, i + 1, time))) queen_del_an(field, i, j, n, time); // if the next queen cannot be placed, then remove the current one and rearrange it
			}
		}
		if (res) break;
	}
	end_queen(field, n, n / 10 + 2); // displaying the final result on the screen
	return res;
}

// the function is called when choosing a visual algorithm for the queens puzzle 
void queen_quest_an() {
	clear(); // cleaning the console for correct operation of the function
	char space = ' ';
	int n; // number of queens
	cout << "Введите колличество ферзей и размерность поля: ";
	cin >> n;

	int** field;
	field = new int* [n];
	for (int j = 0; j < n; j++)
		field[j] = new int[n];

	int space_w = n / 10 + 2;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			field[i][j] = 0;
			cout << string(space_w, space) << field[i][j];
		}
		cout << endl;
	}
	cout << "Цифры на поле - колличество пересечений траекторий ферзей";

	clock_t start_time = clock();
	queen_an(field, n, 0, start_time);
	set_cons(0, n + 2, 1);
	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
	cout << "Ферзи отмечены как 'l'\n\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
}

// maximum search algorithm
int req_max(int* array, int left, int right, fstream & file) {
	int middle, left_part, right_part; // center of the array, left half of the array, right half of the array
	if (left == right) return array[left];
	middle = (left + right) / 2;
	left_part = req_max(array, left, middle, file); // calling a function on the left side of the array
	right_part = req_max(array, middle + 1, right, file); // calling a function on the right side of the array
	file << left_part << " сравнивается с " << right_part << endl; // writing compared elements to a file
	if (left_part > right_part) return left_part; 
	else return right_part;
}

// function is called when choosing an algorithm for finding the maximum array element
void divide_n_conquer() {
	int n; // array size
	cout << "\nВведите размерность массива: ";
	cin >> n;
	cout << endl;
	int* array = new int[n];
	fstream file;
	file.open("req_max.txt");
	for (int i = 0; i < n; i++) array[i] = rand() % 100; // filling an array with random numbers
	int max, left = 0, right = n - 1; // left and right boundaries of the array
	for (int i = 0; i < n; i++) cout << array[i] << " ";
	cout << endl << endl;
	max = req_max(array, left, right, file);
	file.close();
	system("notepad req_max.txt");
	cout << "\nМаксимальный элемент массива: " << max << endl << endl;
}

// function for writing disk movement information to a file
void move_disk( string tower_from, string tower_to, ofstream & file) {
	num_of_sol++;
	file << "\nШаг " << num_of_sol << ": Из " << tower_from << " в " << tower_to;
}

// towers of hanoi algorithm
void move_disks(int disks, string main_tower, string end_tower, string temp_tower, int n, ofstream & file) {
	if (disks == 0) return;
	move_disks(disks - 1, main_tower, temp_tower, end_tower, n, file);
	move_disk(main_tower, end_tower, file);
	move_disks(disks - 1, temp_tower, end_tower, main_tower, n, file);
}

// function is called when choosing an towers of hanoi algorithm
void hanoi_towers() {
	int n; // number of disks
	cout << "\nВведите кол-во дисков: ";
	cin >> n;
	cout << "\nРешается...\n";
	ofstream file;
	file.open("hanoi_steps.txt");
	num_of_sol = 0;
	move_disks(n, "Главная башння", "Конечная башня", "Вспомогательная башня", n + 1, file);
	file.close();
	system("notepad hanoi_steps.txt");
	cout << endl;
}

// function moves the disk on the screen
void move_disk_an(int** towers, int tower_from, int tower_to, int n) {
	int tmp = 0;
	for (int i = 0; i < n; i++) { // search for the topmost disk on the tower from which you want to move the disk
		if (towers[tower_from][i] != 0) {
			tmp = towers[tower_from][i];
			towers[tower_from][i] = 0;
			anim_slow(tower_from + 1, i + 1, '|');
			break;
		}
	}
	for (int i = n - 1; i >= 0; i--) if (towers[tower_to][i] == 0) { // finding a place on top of the other disks on the tower to which you want to move the disk
		towers[tower_to][i] = tmp;
		anim_slow(tower_to + 1, i + 1, tmp);
		break;
	}
}

// towers of hanoi algorithm
void move_disks_an(int** towers, int disks, int main_tower, int end_tower, int temp_tower, int n) {
	if (disks == 0) return;
	move_disks_an(towers, disks - 1, main_tower, temp_tower, end_tower, n);
	move_disk_an(towers, main_tower, end_tower, n);
	move_disks_an(towers, disks - 1, temp_tower, end_tower, main_tower, n);
}

// function is called when choosing an visual towers of hanoi algorithm
void hanoi_towers_an() {
	clear();
	int n; // number of disks
	cout << "Введите кол-во дисков: ";
	cin >> n;
	int** towers;
	towers = new int* [3];
	for (int i = 0; i < 3; i++) towers[i] = new int[n + 1];
	for (int i = 0; i < 3; i++) for (int j = 0; j < n + 1; j++) towers[i][j] = 0;
	for (int i = n + 1; i >= 1; i--) towers[0][i] = i; 
	print(towers, 3, n + 1, '|'); // displaying the initial position of the towers of hanoi on the screen
	move_disks_an(towers, n, 0, 2, 1, n + 1);
	set_cons(0, n + 3, 1);
	for (int i = 0; i < n; i++)
		delete[] towers[i];
	delete[] towers;
}

int main() {
	setlocale(LC_ALL, "RUS"); // Cyrillic localization
	// attention!!!
	// throughout the program, all displayed information is in Russian
	// attention!!!
	string head = "Добро пожаловать в программу по работе с рекурсией"; // text displayed when the program starts
	int time = 30;
	for (int i = 0; i < head.length(); i++) {
		Sleep(time);
		cout << head[i];
	}
	cout << endl << endl;

	string question = "Продолжить?(1 - да, 0 - нет)"; // text with a question about continuing the program
link: // mark
	int c; // user selected option
	c = opt_catch();
	if (c == 1) {
		fibon_one_el();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 2) {
		horse_turn();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 3) {
		horse_turn_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 4) {
		divide_n_conquer();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 5) {
		hanoi_towers();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 6) {
		hanoi_towers_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 7) {
		queen_quest();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 8) {
		queen_quest_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link; // if continuation is selected
		else system("pause"); // end of the program
	}
	if (c == 0) {
		system("pause"); // end of the program
	}
	if (c>8 or c<0) { // if the option is not in the instructions
		time = 300;
		cout << endl;
		string err = "Нет, нет, нет. Пожалуйста следуйте инструкции.";
		for (int i = 0; i < 3; i++)
			cout << err[i];
		Sleep(time);
		for (int i = 3; i < 8; i++)
			cout << err[i];
		Sleep(time);
		for (int i = 8; i < 13; i++)
			cout << err[i];
		Sleep(time);
		time = 6;
		for (int i = 13; i < err.length(); i++) {
			Sleep(time);
			cout << err[i];
		}
		cout << endl;
		goto link;
	}
}
// end of the code