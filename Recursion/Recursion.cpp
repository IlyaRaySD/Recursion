#include <iostream>

using namespace std;

int moves[8][2] = { {2,1}, {2,-1}, {-2,-1}, {-2,1}, {1,2}, {1,-2}, {-1,2}, {-1,-2} };

int horse_move(int** field, int c, int x_h, int y_h, int size) {
	int x2, y2, k = 0, end = 0;
	while (end == 0 and k < size) {
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
	cout << "Введите размерность поля: ";
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
	cout << "Введите координаты коня: \nx = ";
	cin >> x_h;
	cout << "y = ";
	cin >> y_h;
	
	field[x_h-1][y_h-1] = 1;
	
	if (horse_move(field, 2, x_h-1, y_h-1, n)) { 
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				cout << " " << field[i][j];
		}
	}
	else
		cout << "Решение не найдено." << endl;
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
	cout << "Введите порядковый номер конечного элемента последовательности: ";
	cin >> n;
	for (int i = 1; i <= n; i++)
		cout << fibonachi(i) << " ";
	cout << endl;
}



int main() {
	setlocale(LC_ALL, "RUS");
	//fibon_one_el();
	//fibon_sub();
	horse_turn();
}
