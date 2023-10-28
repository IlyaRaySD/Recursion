#include <iostream>
#include "Windows.h"
#include <WinUser.h>
#include <fstream>
#include <stdio.h>
using namespace std;

double TIME = 200;

int moves[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

int num_of_sol = 0;

void clear() { system("cls"); }

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

int opt_catch() {
	int time = 10;
	string option = "\nВыберите интересующий алгоритм:";
	for (int i = 0; i < option.length(); i++) {
		Sleep(time);
		cout << option[i];
	}
	cout << endl;

	string instr = "1 - Последовательность Фибоначи\n2 - Задача о ходе коня\n3 - Задача о ходе коня(наглядно)(запускать только в первую очередь!)\n4 - Поиск максимума, разделяй и властвуй\n5 - Ханойские башни\n6 - Ханойские башни(наглядно)\n7 - Задача о ферзях\n8 - Задача о ферзях(наглядно)(запускать только в первую очередь!)\n0 - Выход из программы\n\nВводите номер: ";
	time = 1;
	for (int i = 0; i < instr.length(); i++) {
		Sleep(time);
		cout << instr[i];
	}
	int choose;
	cin >> choose;
	return choose;
}

void set_cons(int x, int y, int space) {
	COORD position = { (space + 1) * x, y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);
}

template<typename T>
void anim(int x, int y, int space, T num, clock_t start_time) {
	clock_t end_time = clock();
	double search_time = (double)((end_time - start_time)/1000);
	if (search_time > 15)
		TIME /=  1.2;
	if (search_time > 35)
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

template<typename T>
void anim_slow(int x, int y, T num) {
	Sleep(TIME/5);
	set_cons(x, y, 2);
	cout << "\b";
	Sleep(TIME/5);
	cout << num << " ";
	Sleep(TIME/5);
}

template<typename T>
void anim_fast(int x, int y,int space, T num) {
	Sleep(0);
	set_cons(x, y, space);
	cout << "\b";
	Sleep(0);
	cout << num << " ";
	Sleep(0);
}

void end_horse(int** field, int size, int space) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			SetConsoleTextAttribute(console, field[i][j] % 14 + 1);
			cout << field[i][j] << " ";
			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	}
}

void end_queen(int** field, int size, int space) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			if (field[i][j] == -1) {
				SetConsoleTextAttribute(console, 11);
				cout << 'l' << " ";
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else cout << 0 << " ";
		}
	}
}

bool horse_move_an(int** field, int c, int x_h, int y_h, int size, clock_t time) {
	clock_t start_time = clock();
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		x2 = x_h + moves[k][0];
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) {
			if (field[x2][y2] == 0) {
				field[x2][y2] = c;
				SetConsoleTextAttribute(console, c%14 + 1);
				anim(x2 + 1, y2 + 1, size / 10 + 2, c, time);
				SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				if (c < size * size and horse_move_an(field, c + 1, x2, y2, size, time) == 0) {
					field[x2][y2] = 0;
					anim(x2 + 1, y2 + 1, size / 10 + 2, 0, time);
				}
				else {
					end_horse(field, size, size / 10 + 2);
					end = 1;
				}

			}
		}
	}
	return end;
}

void horse_turn_an() {
	char space = ' ';
	int n;
	clear();
	cout << "Введите размерность поля (желательно <7): ";
	cin >> n;

	int** field;
	field = new int* [n];
	for (int i = 0; i < n; i++)
		field[i] = new int[n];

	int space_w = n / 10 + 2;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			field[i][j] = 0;
			cout << string(space_w, space) << field[i][j];
		}
		cout << endl;
	}

	int x_h = 0, y_h = 0;
	cout << "Введите координаты коня: \nx = ";
	cin >> x_h;
	cout << "y = ";
	cin >> y_h;
	cout << "Пожалуйста, подождите, это может занять какое-то время";

	clock_t start_time = clock();

	anim(x_h, y_h, space_w, 1, start_time);

	field[x_h - 1][y_h - 1] = 1;

	if (horse_move_an(field, 2, x_h - 1, y_h - 1, n, 0)) {
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

bool horse_move(int** field, int c, int x_h, int y_h, int size, ofstream &file) {
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		x2 = x_h + moves[k][0];
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) {
			if (field[x2][y2] == 0) {
				field[x2][y2] = c;
				file << c << " ход: " << x2 + 1 << ", " << y2 + 1 << endl;
				if (c < size * size and horse_move(field, c + 1, x2, y2, size, file) == 0) {
					field[x2][y2] = 0;
					file << "Отмена хода " << c << endl;
				}
				else
					end = 1;
			}
		}
	}
	return end;
}

void horse_turn() {
	int n;
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
	ofstream file;
	file.open("solution_knight.txt");
	field[x_h-1][y_h-1] = 1;
	file << "Конь начинает движение с " << x_h << ", " << y_h << endl;
	file << endl << "Ходы коня:\n";
	if (horse_move(field, 2, x_h-1, y_h-1, n, file)) { 
		cout << endl;
		print(field, n, n, 0);
	}
	else
		cout << "\nКонь не смог." << endl;

	
	file.close();
	system("notepad solution_knight.txt");
	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
}

int fibonachi(int num, ofstream& file) {
	file << "\nПопытка найти " << num << "-й элемент\n";
	if (num == 1 or num == 2)
		return 1;
	if (num < 1)
		return 0;
	else
		return (fibonachi(num - 1, file) + fibonachi(num - 2, file));
}

void fibon_one_el() {
	int n;
	cout << "Введите порядковый номер элемнта последовательности : ";
	cin >> n;
	ofstream file;
	file.open("fibon_steps.txt");
	printf("\n%d-й элемент: %u", n, fibonachi(n, file));
	file.close();
	system("notepad fibon_steps.txt");
	cout << endl;
}

bool is_safe(char** desk, int row, int col, int n) {
	for (int i = 0; i < row; i++) {
		if (desk[i][col] == 'l') {
			return 0;
		}
	}
	for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
		if (desk[i][j] == 'l') {
			return 0;
		}
	}
	for (int i = row, j = col; i >= 0 && j < n; i--, j++) {
		if (desk[i][j] == 'l') {
			return 0;
		}
	}
	return 1;
}

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

void try_queen(char** desk, int row, int n, ofstream& file) {
	if (row == n) {
		num_of_sol++;
		print_sol(desk, n, file);
		return;
	}
	for (int i = 0; i < n; i++) {
		if (is_safe(desk, row, i, n)) {
			desk[row][i] = 'l';
			try_queen(desk, row + 1, n, file);
			desk[row][i] = '0';
		}
	}
}

void queen_quest() {
	int n;
	cout << "Введите количество ферзей: ";
	cin >> n;
	cout << endl << endl;
	ofstream file;
	file.open("solutions_queen.txt");
	char** desk = new char* [n];
	for (int i = 0; i < n; i++) desk[i] = new char[n];
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) desk[i][j] = '0';
	num_of_sol = 0;
	try_queen(desk, 0, n, file);
	file << "Ферзи отмечены как 'l'";
	file.close();
	cout << "Количество решений: " << num_of_sol << endl << endl;
	system("notepad solutions_queen.txt");
	for (int i = 0; i < n; i++)
		delete[] desk[i];
	delete[] desk;
}

void queen_stand_an(int** field, int x, int y, int n, clock_t time) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < n; i++) {
		++field[i][y];
		anim_fast(i+1, y + 1, n/10+2, field[i][y]);
		++field[x][i];
		anim_fast(x+1, i+1, n/10+2, field[i][y]);
		int diag = y - x + i;
		if (diag >= 0 and diag < n) {
			++field[i][diag];
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
		diag = y + x - i;
		if (diag >= 0 and diag < n) {
			++field[i][diag];
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
	}
	field[x][y] = -1;
	SetConsoleTextAttribute(console, 11);
	anim(x+1, y+1, n/10+2, 'l', time);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void queen_del_an(int** field, int x, int y, int n, clock_t time) {
	for (int i = 0; i < n; i++) {
		--field[i][y];
		anim_fast(i+1, y+1, n/10+2, field[i][y]);
		--field[x][i];
		anim_fast(x+1, i+1, n/10+2, field[x][i]);
		int diag = y - x + i;
		if (diag >= 0 and diag < n) {
			--field[i][diag];
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
		diag = y + x - i;
		if (diag >= 0 and diag < n) {
			--field[i][diag];
			anim_fast(i+1, diag+1, n/10+2, field[i][diag]);
		}
	}
	field[x][y] = 0;
	anim(x+1, y+1, n/10+2, 0, time);
}

bool queen_an(int** field, int n, int i, clock_t time) {
	bool res = 0;
	for (int j = 0; j < n; j++) {
		if (field[i][j] == 0) {
			queen_stand_an(field, i, j, n, time);
			if (i == n - 1) res = 1;
			else {
				if (!(res = queen_an(field, n, i + 1, time))) queen_del_an(field, i, j, n, time);
			}
		}
		if (res) break;
	}
	end_queen(field, n, n / 10 + 2);
	return res;
}

void queen_quest_an() {
	clear();
	char space = ' ';
	int n;
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

int req_max(int* array, int left, int right) {
	int middle, left_part, right_part;
	if (left == right) return array[left];
	middle = (left + right) / 2;
	left_part = req_max(array, left, middle);
	right_part = req_max(array, middle + 1, right);
	cout << left_part << " сравнивается с " << right_part << endl;
	if (left_part > right_part) return left_part;
	else return right_part;
}

void divide_n_conquer() {
	int n;
	cout << "\nВведите размерность массива: ";
	cin >> n;
	cout << endl;
	int* array = new int[n];
	for (int i = 0; i < n; i++) array[i] = rand() % 100;
	int max, left = 0, right = n - 1;
	for (int i = 0; i < n; i++) cout << array[i] << " ";
	cout << endl << endl;
	max = req_max(array, left, right);
	cout << "\nМаксимальный элемент массива: " << max << endl << endl;
}

void move_disk( string tower_from, string tower_to, ofstream & file) {
	file << "\n Из " << tower_from << " в " << tower_to;
}

void move_disks(int disks, string main_tower, string end_tower, string temp_tower, int n, ofstream & file) {
	if (disks == 0) return;
	move_disks(disks - 1, main_tower, temp_tower, end_tower, n, file);
	move_disk(main_tower, end_tower, file);
	move_disks(disks - 1, temp_tower, end_tower, main_tower, n, file);
}

void hanoi_towers() {
	int n;
	cout << "\nВведите кол-во дисков: ";
	cin >> n;
	cout << "\nРешается...\n";
	ofstream file;
	file.open("hanoi_steps.txt");
	move_disks(n, "Главная башння", "Конечная башня", "Вспомогательная башня", n + 1, file);
	file.close();
	system("notepad hanoi_steps.txt");
	cout << endl;
}

void move_disk_an(int** towers, int tower_from, int tower_to, int n) {
	int tmp = 0;
	for (int i = 0; i < n; i++) {
		if (towers[tower_from][i] != 0) {
			tmp = towers[tower_from][i];
			towers[tower_from][i] = 0;
			anim_slow(tower_from + 1, i + 1, '|');
			break;
		}
	}
	for (int i = n - 1; i >= 0; i--) if (towers[tower_to][i] == 0) {
		towers[tower_to][i] = tmp;
		anim_slow(tower_to + 1, i + 1, tmp);
		break;
	}
}

void move_disks_an(int** towers, int disks, int main_tower, int end_tower, int temp_tower, int n) {
	if (disks == 0) return;
	move_disks_an(towers, disks - 1, main_tower, temp_tower, end_tower, n);
	move_disk_an(towers, main_tower, end_tower, n);
	move_disks_an(towers, disks - 1, temp_tower, end_tower, main_tower, n);
}

void hanoi_towers_an() {
	clear();
	int n;
	cout << "Введите кол-во дисков: ";
	cin >> n;
	int** towers;
	towers = new int* [3];
	for (int i = 0; i < 3; i++) towers[i] = new int[n + 1];
	for (int i = 0; i < 3; i++) for (int j = 0; j < n + 1; j++) towers[i][j] = 0;
	for (int i = n + 1; i >= 1; i--) towers[0][i] = i;
	print(towers, 3, n + 1, '|');
	move_disks_an(towers, n, 0, 2, 1, n + 1);
	set_cons(0, n + 3, 1);
	for (int i = 0; i < n; i++)
		delete[] towers[i];
	delete[] towers;
}

int main() {
	setlocale(LC_ALL, "RUS");

	string head = "Добро пожаловать в лабу номер 2";
	int time = 30;
	for (int i = 0; i < head.length(); i++) {
		Sleep(time);
		cout << head[i];
	}
	cout << endl << endl;

	string question = "Продолжить?(1 - да, 0 - нет)";
link:

	int c;
	c = opt_catch();
	if (c == 1) {
		fibon_one_el();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 2) {
		horse_turn();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 3) {
		horse_turn_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 4) {
		divide_n_conquer();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 5) {
		hanoi_towers();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 6) {
		hanoi_towers_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 7) {
		queen_quest();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 8) {
		queen_quest_an();
		bool ask = 0;
		cout << question << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 0) {
		system("pause");
	}
	if (c>8 or c<1) {
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