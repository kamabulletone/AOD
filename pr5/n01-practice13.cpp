#include <iostream>

#include <string>

#include <algorithm>
// Вариант 4.

using namespace std;

bool orientalgraph = 1; // ориентированный граф или нет

int elems = 1;

struct Bond;

struct Node {

	string name;

	Bond* ways;

	int bondcount;

};

struct Bond {

	int way;

	Node* child;

};

void SetBond(string parentname, Node* child, int way, Node* root);

void AddElem(string name, Node* root, string* parents, int counter, int* ways) {

	Node* newelem = new Node;

	newelem->name = name;

	newelem->bondcount = 0;

	newelem->ways = new Bond;

	for (int i = 0; i < counter; i++) {

		SetBond(parents[i], newelem, ways[i], root);

	}

	elems++;

}

Node* Search(string name, Node* root) {

	if (name == root->name)

		return root;

	for (int i = 0; i < root->bondcount; i++) {

		if (Search(name, root->ways[i].child) != NULL) {

			return Search(name, root->ways[i].child);

		}

	}

	return NULL;

}

void SetBond(string parentname, Node* child, int way, Node* root) {

	Node* parent = Search(parentname, root);

	Bond* newarr = new Bond[parent->bondcount + 1];

	for (int i = 0; i < parent->bondcount; i++) {

		newarr[i] = parent->ways[i];

	}

	newarr[parent->bondcount].child = child;

	newarr[parent->bondcount].way = way;

	parent->bondcount += 1;

	parent->ways = newarr;

}

Node* CreateGraph() {

	Node* root = new Node;

	root->name = "1";

	root->bondcount = 0;

	root->ways = new Bond;

	return root;

}

string* destruct;

int restruct = 0;

Node* DefaultGraph() {

	Node* root = CreateGraph();

	string* parents;

	int* ways;


	parents = new string[1];

	parents[0] = "1";

	ways = new int[1];

	ways[0] = 23;

	AddElem("2", root, parents, 1, ways);

	parents = new string[2];

	parents[0] = "1";

	parents[1] = "2";

	ways = new int[2];

	ways[0] = 12;

	ways[1] = 25;

	AddElem("3", root, parents, 2, ways);

	parents = new string[1];

	parents[0] = "3";

	ways = new int[1];

	ways[0] = 18;

	AddElem("4", root, parents, 1, ways);

	parents = new string[1];

	parents[0] = "2";

	ways = new int[1];

	ways[0] = 22;

	AddElem("5", root, parents, 1, ways);

	parents = new string[2];

	parents[0] = "4";

	parents[1] = "5";

	ways = new int[2];

	ways[0] = 20;

	ways[1] = 23;

	AddElem("6", root, parents, 2, ways);

	parents = new string[2];

	parents[0] = "5";

	parents[1] = "6";

	ways = new int[2];

	ways[0] = 14;

	ways[1] = 24;

	AddElem("7", root, parents, 2, ways);

	parents = new string[2];

	parents[0] = "2";

	parents[1] = "7";

	ways = new int[2];

	ways[0] = 35;

	ways[1] = 16;

	AddElem("8", root, parents, 2, ways);

	return root;

}

void GraphOut(Node* root) {

	for (int i = 0; i < restruct; i++) {

		if (destruct[i] == root->name)

			return;

	}

	cout << "Вершина: " << root->name << ", дочерние вершины: ";

	for (int i = 0; i < root->bondcount; i++) {

		cout << i + 1 << ". " << root->ways[i].child->name << " (Длина пути: " << root->ways[i].way << "); ";

	}

	cout << endl;

	string* checker1 = new string[restruct + 1];

	for (int i = 0; i < restruct; i++) {

		checker1[i] = destruct[i];

	}

	checker1[restruct] = root->name;

	restruct++;

	destruct = checker1;

	for (int i = 0; i < root->bondcount; i++) {

		GraphOut(root->ways[i].child);

	}

}

void Floid(Node* root, string name1, string name2) {

	int** arr = new int*[elems];

	for (int i = 0; i < elems; i++) {

		arr[i] = new int[elems];

	}

	for (int i = 0; i < elems; i++) {

		for (int j = 0; j < elems; j++) {

			arr[i][j] = 2000000;

		}

	}

	for (int i = 0; i < elems; i++) {

		string bruh = to_string(i + 1);

		Node* name = Search(bruh, root);

		for (int j = 0; j < name->bondcount; j++) {

			arr[i][stoi(name->ways[j].child->name) - 1] = name->ways[j].way;

		}

	}

	if (!orientalgraph) {

		for (int i = 0; i < elems; i++) {

			for (int j = 0; j < elems; j++) {

				if (arr[i][j] < arr[j][i]) {

					arr[j][i] = arr[i][j];

				}

			}

		}

	}

	for (int k = 0; k < elems; k++) {

		for (int i = 0; i < elems; i++) {

			for (int j = 0; j < elems; j++) {

				arr[i][j] = min(arr[i][j], arr[i][k] + arr[k][j]);

			}

		}

	}

	int name01 = stoi(name1) - 1;

	int name02 = stoi(name2) - 1;

	if (arr[name01][name02] != 2000000)

		cout << "Кратчайший путь между " << name1 << " и " << name2 << ": " << arr[name01][name02] << endl;

	else cout << "Пути не существует" << endl;

}

int main() {

	setlocale(LC_ALL, "rus");

	Node* root = NULL;

	string a = "";

	int graphname = 2;

	string pp = "";

	string founder;

	string name;

	int ii = 0;

	bool newgraph = true;

	int counter = 0;

	int parentsize = 0;

	int namesize = 0;

	string name1;

	bool chk2 = true;

	bool chk3 = true;

	string name2;

	string* parents = NULL;

	int* ways = NULL;

	string* parents1;

	int* ways1;

	int s = 0;

	int chk = 0;

	bool chk1 = true;

	string ss;

	int b6 = 0;

	while (true) {

		while (a != "1" && a != "2" && a != "3" && a != "4" && a != "5") { //Проверка ввода корректного значения

			if (s != 0) { //Условие вывода сообщения об ошибке ввода

				cout << "Введено неверное значение \n";

			}

			cout << "Выберите режим работы программы: \n 1. Создание графа из индивидуального варианта \n 2. Ввод произвольного графа с клавиатуры \n 3. Нахождение кратчайшего пути методом Флойда \n 4. Вывод зависимостей графа \n 5. Выход \n";

			cin >> a;

			s++;

		}

		int b = stoi(a);

		s = 0;

		a = "";

		switch (b) {

		case 1: //Создание графа из индивидуального варианта

			root = DefaultGraph();

			cout << "Граф создан" << endl;

			break;

		case 2: //Ввод произвольного графа с клавиатуры

			cout << "Перезаписать существующий граф? 1 - Да, 0 - Нет" << endl;

			cin >> newgraph;

			if (newgraph) {

				root = NULL;

				graphname = 2;

				parentsize = 0;

				namesize = 0;

				counter = 0;

				elems = 1;

			}

			while (chk1) {

				cout << "Выберите действие: \n 1. Создание корневого элемента графа(Перезапись корневого эелемента приведет к удалению графа) \n 2. Добавление новой вершины графа \n 3. Назад \n";

				cin >> chk;

				switch (chk) {

				case 1:

					root = CreateGraph();

					cout << "Граф создан, имя корневого элемента '1'" << endl;

					break;

				case 2:

					if (root != NULL) {

						name = to_string(graphname);

						cout << "Имя создаваемой вершины '" << graphname << "'" << endl;

						graphname++;

						while (chk2) {

							cout << "Введите имя родителя вершины(Введите END для того, чтобы перестать указывать родителей):" << endl;

							cin >> pp;

							if (pp != "END") {

								parents1 = new string[parentsize + 1];

								for (int i = 0; i < parentsize; i++) {

									parents1[i] = parents[i];

								}

								parents1[parentsize] = pp;

								parents = parents1;

								parentsize++;

								cout << "Введите длину пути из введенного родителя в создаваемую вершину:" << endl;

								cin >> ii;

								ways1 = new int[namesize + 1];

								for (int i = 0; i < namesize; i++) {

									ways1[i] = ways[i];

								}

								ways1[namesize] = ii;

								ways = ways1;

								namesize++;

								counter++;

							}

							else chk2 = false;

						}

						chk2 = true;

						AddElem(name, root, parents, counter, ways);

						cout << "Вершина добавлена" << endl;

					}

					else cout << "Граф не создан" << endl;

					break;

				case 3:

					chk1 = false;

					break;

				default:

					cout << "Введено неверное значение \n";

				}

			}

			chk1 = true;

			break;

		case 3: //Нахождение кратчайшего пути методом Флойда

			cout << "Введите имя вершины отправления:" << endl;

			cin >> name1;

			cout << "Введите имя вершины прибытия:" << endl;

			cin >> name2;

			if (root != NULL)

				Floid(root, name1, name2);

			else cout << "Граф не создан" << endl;

			break;

		case 4: //Вывод зависимостей графа
			destruct = NULL;

			restruct = 0;

			if (root != NULL)

				GraphOut(root);

			else cout << "Граф не создан" << endl;

			break;

		case 5: //Выход

			return 0;

			break;

		default: //Перестраховка при вводе неверного значения

			cout << "Введено неверное значение \n";

}

}

}