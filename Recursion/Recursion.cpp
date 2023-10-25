﻿#include <iostream>
#include "Windows.h"
#include <WinUser.h>
using namespace std;

double TIME = 200;

int moves[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

void clear() { system("cls"); }

int opt_catch() {
	int time = 10;
	string option = "\nВыберите интересующий алгоритм:";
	for (int i = 0; i < option.length(); i++) {
		Sleep(time);
		cout << option[i];
	}
	cout << endl;

	string instr = "1 - Последовательность Фибоначи\n2 - Задача о ходе коня\n3 - Задача о ходе коня(наглядно)\n4 - Разделяй и влавствуй, поиск максимума\n5 - Задача о ферзях\n6 - Задача о ферзях(наглядно)\n7 - Выход из программы\n\nВводите номер: ";
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
	if (search_time > 7)
		TIME /=  1.2;
	if (search_time > 50)
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

void end_anim(int** field, int size, int space) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			cout << field[i][j] << " ";
		}
	}
}

void end_queen(int** field, int size, int space) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			set_cons(i + 1, j + 1, space);
			cout << "\b";
			if (field[i][j] == -1) cout << 'l' << " ";
			else cout << 0 << " ";
		}
	}
}

bool horse_move_an(int** field, int c, int x_h, int y_h, int size, clock_t time) {
	clock_t start_time = clock();
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		x2 = x_h + moves[k][0];
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) {
			if (field[x2][y2] == 0) {
				field[x2][y2] = c;
				anim(x2 + 1, y2 + 1, size / 10 + 2, c, time);
				if (c < size * size and horse_move_an(field, c + 1, x2, y2, size, time) == 0) {
					field[x2][y2] = 0;
					anim(x2 + 1, y2 + 1, size / 10 + 2, 0, time);
				}
				else {
					end_anim(field, size, size / 10 + 2);
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
	cout << "Пожалуйста, подождите, это может занять максимум 50 секунд";

	clock_t start_time = clock();

	anim(x_h, y_h, space_w, 1, start_time);

	field[x_h - 1][y_h - 1] = 1;

	if (horse_move_an(field, 2, x_h - 1, y_h - 1, n, 0)) {
		cout << endl;
		set_cons(0, n + 6, 1);
		cout << "Конь смог.\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
	}
	else {
		set_cons(0, n + 6, 1);
		cout << "\nКонь не смог.\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
	}
	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
	
}

bool horse_move(int** field, int c, int x_h, int y_h, int size) {
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < 8) {
		x2 = x_h + moves[k][0];
		y2 = y_h + moves[k][1];
		k++;
		if (x2 >= 0 and y2 >= 0 and x2 < size and y2 < size) {
			if (field[x2][y2] == 0) {
				field[x2][y2] = c;
				if (c < size*size and horse_move(field, c + 1, x2, y2, size) == 0)
					field[x2][y2] = 0;
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
	
	field[x_h-1][y_h-1] = 1;
	
	if (horse_move(field, 2, x_h-1, y_h-1, n)) { 
		cout << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				cout << " " << field[i][j];
			cout << endl;
		}
	}
	else
		cout << "\nКонь не смог." << endl;

	for (int i = 0; i < n; i++)
		delete[] field[i];
	delete[] field;
}

int fibonachi(int num) {
	if (num == 1 or num == 2)
		return 1;
	if (num < 1)
		return 0;
	else
		return (fibonachi(num - 1) + fibonachi(num - 2));
}

void fibon_one_el() {
	int n;
	cout << "Введите порядковый номер элемнта последовательности : ";
	cin >> n;
	printf("%d-й элемент: %u", n, fibonachi(n));
	cout << endl;
}

void fibon_sub() {
	int n;
	cout << "\nВведите порядковый номер конечного элемента последовательности: ";
	cin >> n;
	cout << endl;
	for (int i = 1; i <= n; i++)
		cout << fibonachi(i) << " ";
	cout << endl << endl;
}

void queen_del(int** field, int x, int y, int n) {
	for (int i = 0; i < n; i++) {
		field[i][y]--;
		field[x][i]--;
		int diag = y - x + i;
		if (diag >= 0 and diag < n) field[i][diag]--;
		diag = y + x - i;
		if (diag >= 0 and diag < n) field[i][diag]--;
	}
	field[x][y] = 0;
}

void queen_stand(int** field, int x, int y, int n) {
	for (int i = 0; i < n; i++) {
		field[i][y]++;
		field[x][i]++;
		int diag = y - x + i;
		if (diag >= 0 and diag < n) field[i][diag]++;
		diag = y + x - i;
		if (diag >= 0 and diag < n) field[i][diag]++;
	}
	field[x][y] = -1;
}

bool queen(int** field, int n, int i) {
	bool res = 0;
	for (int j = 0; j < n; j++) {
		if (field[i][j] == 0) {
			queen_stand(field, i, j, n);
			if (i == n - 1) res = 1;
			else {
				if (!(res = queen(field, n, i + 1))) queen_del(field, i, j, n);
			}
		}
		if (res) break;
	}
	return res;
}

void queen_quest() {
	int n;
	cout << "\nВведите колличество ферзей и размерность поля: ";
	cin >> n;
	cout << endl;

	int** field;
	field = new int* [n];
	for (int j = 0; j < n; j++)
		field[j] = new int[n];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			field[i][j] = 0;
	}

	queen(field, n, 0);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (field[i][j] == -1) cout << "l ";
			else cout << "0 ";
		}
		cout << endl;
	}
	cout << "Ферзи отмечены как 'l'" << endl << endl;
}

void queen_stand_an(int** field, int x, int y, int n, clock_t time) {
	for (int i = 0; i < n; i++) {
		++field[i][y];
		anim(i+1, y + 1, n/10+2, field[i][y], time);
		++field[x][i];
		anim(x+1, i+1, n/10+2, field[i][y], time);
		int diag = y - x + i;
		if (diag >= 0 and diag < n) {
			++field[i][diag];
			anim(i+1, diag+1, n/10+2, field[i][diag], time);
		}
		diag = y + x - i;
		if (diag >= 0 and diag < n) {
			++field[i][diag];
			anim(i+1, diag+1, n/10+2, field[i][diag], time);
		}
	}
	field[x][y] = -1;
	anim(x+1, y+1, n/10+2, 'l', time);
}

void queen_del_an(int** field, int x, int y, int n, clock_t time) {
	for (int i = 0; i < n; i++) {
		--field[i][y];
		anim(i+1, y+1, n/10+2, field[i][y], time);
		--field[x][i];
		anim(x+1, i+1, n/10+2, field[x][i], time);
		int diag = y - x + i;
		if (diag >= 0 and diag < n) {
			--field[i][diag];
			anim(i+1, diag+1, n/10+2, field[i][diag], time);
		}
		diag = y + x - i;
		if (diag >= 0 and diag < n) {
			--field[i][diag];
			anim(i+1, diag+1, n/10+2, field[i][diag], time);
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

	clock_t start_time = clock();
	queen_an(field, n, 0, start_time);
	set_cons(0, n + 1, 1);
	cout << "Ферзи отмечены как 'l'\nЕсли у вас всё перед глазами пронеслось, то попробуйте перезапустить программу и сразу выбрать этот пункт снова" << endl << endl;
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

link:

	int c;
	c = opt_catch();
	if (c == 1) {
		fibon_sub();
		bool ask = 0;
		cout << "Продолжить?(1 - да, 0 - нет)" << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 2) {
		horse_turn();
		bool ask = 0;
		cout << "Продолжить?(1 - да, 0 - нет)" << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 3) {
		horse_turn_an();
		goto link;
	}
	if (c == 4) {
		bool ask = 0;
		cout << "Продолжить?(1 - да, 0 - нет)" << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 5) {
		queen_quest();
		bool ask = 0;
		cout << "Продолжить?(1 - да, 0 - нет)" << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 6) {
		queen_quest_an();
		bool ask = 0;
		cout << "Продолжить?(1 - да, 0 - нет)" << endl;
		cin >> ask;
		if (ask == 1) goto link;
		else system("pause");
	}
	if (c == 7) {
		system("pause");
	}
	if (c>7 or c<1) {
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