
#include <math.h>;
#include <iostream>;
#include <queue>;
#include <ctime>

using namespace std;

int getMax(int arr[], int n) {
	int mx = arr[0];
	for (int i = 1; i < n; i++) {
		if (arr[i] > mx) {
			mx = arr[i];
		}
	}
	return mx;
}

void cousort(int* arr, int n, int mult) {
	if (*arr == NULL)
		return;
	int* out;
	out = new int[n];
	int i, count[10] = { 0 };
	for (i = 0; i < n; i++) {
		count[(arr[i] / mult)%10]++;
	}
	for (i = 1; i < 10; i++) {
		count[i] += count[i - 1];
	}
	for (i = n - 1; i >= 0; i--) {
		out[count[(arr[i] / mult) % 10] - 1] = arr[i];
		count[(arr[i] / mult) % 10]--;
	}
	for (i = 0; i < n; i++) {
		arr[i] = out[i];
	}
}

void gosort(int arr[], int n) {
	int m = getMax(arr, n);
	for (int mult = 1; m / mult > 0; mult *= 10) {
		cousort(arr, n, mult);
	}
}

queue<int> dob(queue<int>b, int k) {
	b.push(k);
	return(b);
}

int retint(queue<int> b, int k) {
	int ty = -1;
	queue<int> c = b;
	for (int i = -1; i < k; i++) {
		ty = c.front();
		c.pop();
	}
	return ty;
}

queue<int> delint(queue<int> b, int k) {
	int ty;
	queue<int> c;
	for (int i = 0; i < c.size(); i++) {
		int z = b.front();
		if (z != k)
			c.push(z);
		b.pop();
	}
	return c;
}

bool emp(queue<int> b) {
	return b.empty();
}

bool nemp(queue<int> b) {
	return !b.empty();
}

int main(){

	srand(0);
	int i, j;
	queue<queue<int>> a;
	int n;
	for (i = 0; i < 10; i++) {
		queue <int> v;
		n = rand() % 100;
		for (j = 0; j < n; j++) {
			v.push(rand());
		}
		a.push(v);
	}
	/*for (i = 0; i < 10; i++) {

	}*/
	int* x;
	x = new int[10];
	for (i = 0; i < 10; i++) {
		x = new int[10];
		for (j = 0; j < 10; j++) {
			x[j] = retint(a.front(), j);
		}
		gosort(x, 10);
		for (int r = 0; r < 10; r++) {
			cout << x[r] << endl;
		}
		cout << endl;
	}
	
}
