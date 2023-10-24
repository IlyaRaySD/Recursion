#include <iostream>
#include "Windows.h"

using namespace std;

int moves[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };

int opt_catch() {
	int time = 30;
	string option = "\nВыберите интересующий алгоритм:";
	for (int i = 0; i < option.length(); i++) {
		Sleep(time);
		cout << option[i];
	}
	cout << endl;

	string instr = "1 - Последовательность Фибоначи\n2 - Задача о ходе коня\n3 - Разделяй и влавствуй, поиск максимума\n4 - Задача о ферзях\n5 - Выход из программы\n\nВводите номер: ";
	time = 6;
	for (int i = 0; i < instr.length(); i++) {
		Sleep(time);
		cout << instr[i];
	}
	int choose;
	cin >> choose;
	return choose;
}

int horse_move(int** field, int c, int x_h, int y_h, int size) {
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
	for (int i = 1; i <= n; i++)
		cout << fibonachi(i) << " ";
	cout << endl << endl;
}

int main() {
	setlocale(LC_ALL, "RUS");

	string head = "Добро пожаловать в лабу номер 2";
	int time = 40;
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
		goto link;
	}
	if (c == 2) {
		horse_turn();
		goto link;
	}
	if (c == 3) {
		cout << "\nЯ пока не сделал это" << endl;
		goto link;
	}
	if (c == 4) {
		cout << "\nЯ пока не сделал это" << endl;
		goto link;
	}
	if (c == 5) {
		system("pause");
	}
	if (c>5 or c<1) {
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
