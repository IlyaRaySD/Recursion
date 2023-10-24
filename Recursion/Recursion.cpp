#include <iostream>

using namespace std;

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
	printf("%d-й элемент: %zu", n, fibonachi(n));
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
	fibon_one_el();
	fibon_sub();
}
