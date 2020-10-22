#define default_size 8 //определение стандартного размера хештаблицы

#define rehash_size 0.75 // коэффициент заполнения таблицы для хеширования

#include<iostream>

#include<string>

#include <fstream>

using namespace std;

class Person // класс, которым заполняется хештаблица

{

public:

	string number;

	string name;

	string adress;

	Person(string number, string name, string adress) {

		this->number = number;

		this->name = name;

		this->adress = adress;

	}

};

int MainHash(const string& s, int t, const int key) // функция хеширования

{

	int hash_result = 0;

	for (int i = 0; s[i] != 0; ++i)

	{

		hash_result = (key * hash_result + s[i]) % t;

	}

	hash_result = (hash_result * 2 + 1) % t;

	return hash_result;

}

int Hashfunc1(const string& s, int t) //первый вариант функции хеширования

{

	return MainHash(s, t, t - 1);

};

int Hashfunc2(const string& s, int t) //второй вариант функции хеширования

{

	return MainHash(s, t, t + 1);

};

template <class T> // шаблон класса

class HashTable {

public:

	struct Node //узел хештаблицы

	{

		T value;

		bool state;

		Node(const T& value_) : value(value_), state(true) {}

	};

	Node** arr;

	int size;

	int bufsize;

	int realsize;

	void Resize() //изменение размера таблицы при ее заполнении на опреденное число

	{

		bufsize *= 2;

		realsize = 0;

		size = 0;

		Node** arr2 = new Node *[bufsize];

		for (int i = 0; i < bufsize; ++i)

			arr2[i] = NULL;

		swap(arr, arr2);

		for (int i = 0; i < (bufsize / 2); i++)

		{

			if (arr2[i] && arr2[i]->state)

				Add(arr2[i]->value);

		}

		for (int i = 0; i < (bufsize / 2); i++)

			if (arr2[i])

				delete arr2[i];

		delete[] arr2;

	}

	void Rehash() //повторное хеширование таблицы

	{

		realsize = 0;

		size = 0;

		Node** arr2 = new Node *[bufsize];

		for (int i = 0; i < bufsize; i++) //инициализация новой таблицы

			arr2[i] = NULL;

		swap(arr, arr2); //замена таблицы

		for (int i = 0; i < bufsize; i++) //перенос элементов

		{

			if (arr2[i] && arr2[i]->state)

				Add(arr2[i]->value);

		}

		for (int i = 0; i < bufsize; i++) // удаление временной таблицы

			if (arr2[i])

				delete arr2[i];

		delete[] arr2;

	}

	HashTable() //конструктор таблицы

	{

		bufsize = default_size;

		size = 0;

		realsize = 0;

		arr = new Node *[bufsize];

		for (int i = 0; i < bufsize; i++) // пустой массив

			arr[i] = NULL;

	}

	~HashTable() //деструктор таблицы

	{

		for (int i = 0; i < bufsize; i++) //освобождение памяти

			if (arr[i])

				delete arr[i];

		delete[] arr;

	}

	bool Add(const T& value) //добавление элемента в таблицу

	{

		if (size + 1 > int(rehash_size * bufsize)) //выполнениерехеширования или

			Resize();

		else if (realsize > 2 * size)

			Rehash();

		int h1 = Hashfunc1(value.number, bufsize); //результат работы первой

		int h2 = Hashfunc2(value.number, bufsize); //результат работы второй

		int i = 0;

		int fdel = -1;

		while (arr[h1] != NULL && i < bufsize) // поиск непустого места в

		{

			if (arr[h1]->value.number == value.number && arr[h1]->state)

				return false;

			if (!arr[h1]->state && fdel == -1)

				fdel = h1;

			h1 = (h1 + h2) % bufsize;

			i++;

		}

		if (fdel == -1)

		{

			arr[h1] = new Node(value);

			++realsize; //создание нового узла

		}

		else

		{

			arr[fdel]->value.number = value.number;

			arr[fdel]->state = true;

		}

		size--; //уменьшение пустого размера

		return true;

	}

	bool Remove(const T& value) //удаление элемента из таблицы

	{

		int h1 = Hashfunc1(value.number, bufsize);

		int h2 = Hashfunc2(value.number, bufsize);

		int i = 0;

		while (arr[h1] != NULL && i < bufsize) // проход по всем элементам

		{

			if (arr[h1]->value.number == value.number && arr[h1]->state)

			{

				arr[h1]->state = false;

				size--;

				this->InBin();

				return true;

			}

			h1 = (h1 + h2) % bufsize;

			i++;

		}

		return false;

	}

	bool Find(const T& value) // нахождение элемента в таблице

	{

		int h1 = Hashfunc1(value.number, bufsize);

		int h2 = Hashfunc2(value.number, bufsize);

		int i = 0;

		while (arr[h1] != NULL && i < bufsize) // проход по всем элементам

		{

			if (arr[h1]->value.number == value.number && arr[h1]->state)

				return true;

			h1 = (h1 + h2) % bufsize;

			i++;

		}

		return false;

	}

	void Out() { // вывод всех элементов таблицы

		for (int i = 0; i < bufsize; i++) {

			if (arr[i] != NULL) {

				cout << arr[i]->value.number << " " << arr[i]->value.adress << " " << arr[i]->value.name << "\n";

			}

		}

		this->InBin();

	}

	void InBin() {

		string buf = "";

		for (int i = 0; i < bufsize; i++) {

			if (arr[i] != NULL) {

				buf += arr[i]->value.number + " " + arr[i]->value.adress + " " + arr[i]->value.name + "\n";

			}

		}

		fstream fs("out.bin", std::ios::out | std::ios::binary);

		if (fs.is_open())

		{

			unsigned strLen = buf.size();

			fs.write((char*)&strLen, sizeof(unsigned));

			fs.write((char*)buf.c_str(), buf.size());

			fs.close();

		}

	}

};

int main(void) {

	HashTable<Person> ht; //создание таблицы

	string x1, x2, x3;

	ifstream f;

	f.open("file.txt");

	while (!f.eof()) {

		string str;

		f >> x1;

		f >> x2;

		f >> x3;

		ht.Add(Person(x1, x2, x3)); //добавление элемента в таблицу

	}

	f.close();

	ht.Out(); //вывод таблицы

	system("pause");

}