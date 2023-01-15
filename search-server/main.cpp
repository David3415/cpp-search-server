// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:

#include <iostream>

using namespace std;

int main() {
	int res = 0;
	for (int i = 1; i <= 1000; i++) {
		if (i % 10 == 3) {
			res++;
			continue;
		}
		else if (i % 10 / 10 == 3) {
			res++;
			continue;
		}
		else if (i % 100 / 100 == 3) {
			res++;
			continue;
		}
	}
	cout << res;
}
// Закомитьте изменения и отправьте их в свой репозиторий.
