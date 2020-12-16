#include<iostream>
#include<string>
using namespace std;
int W, K;
int curP;
int curW, bestW, lostW;
int MaxW;
void culc(int j, int* price, int* weight, int* x, int* y) {
	if (j == K) {
		if (bestW < curW && curP <= W) {
			for (int i = 0; i < K; ++i) y[i] = x[i];
			bestW = curW;
		}
		return;
	}
	lostW += price[j];
	if (bestW <= MaxW - lostW)
		culc(j + 1, price, weight, x, y);
	lostW -= price[j];
	curP += weight[j];  curW += price[j];
	if (curP <= W) {
		x[j] = 1;
		culc(j + 1, price, weight, x, y);
		x[j] = 0;
	}
	curP -= weight[j]; curW -= price[j];
}
int main() {
	setlocale(LC_ALL, "rus");
	cout << "Введите грузоподъемность рюкзака = ";
	cin >> W;
	cout << "Введите количество предметов = ";
	cin >> K;
	string* s = new string[K];
	int* price = new int[K];
	int* weight = new int[K];
	int* x = new int[K];
	int* y = new int[K];
	int test = 0;
	for (int i = 0; i < K; ++i) {
		x[i] = y[i] = 0;
		cin >> s[i] >> weight[i] >> price[i];
	}
	for (int i = 0; i < K; i++) MaxW += price[i];
	culc(0, price, weight, x, y);
	for (int i = 0; i < K; ++i) test += y[i] * weight[i];
	cout << "Предметы:" << endl;
	for (int i = 0; i < K; ++i)if (y[i]) cout << s[i] << endl;
	cout << "Стоимость = " << bestW << endl << "Масса = " << test << endl;
}
