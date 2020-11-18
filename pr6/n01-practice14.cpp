#include<iostream>
#include<algorithm>
#include<string>
#include<math.h>
using namespace std;
struct elem {
	char symbol = '*';
	int num = 0;
	float chance = 0;
};
struct mas {
	elem* table;
	int ctr;
};
mas Tablein(string a) {
	bool chk = false;
	int ctr = 0;
	elem* table2 = new elem[0];
	elem* table = new elem[a.size()];
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < i; j++) {
			if (a[i] == table[j].symbol) {
				chk = true;
				table[j].num++;
			}
		}
		if (!chk) {
			table[i].symbol = a[i];
			table[i].num++;
		}
		chk = false;
	}
	for (int i = 0; i < a.size(); i++) {
		if (table[i].symbol != '*') {
			ctr++;
			elem* table1 = new elem[ctr];
			for (int j = 0; j < ctr; j++) {
				table1[j].symbol = table2[j].symbol;
				table1[j].num = table2[j].num;
			}
			table1[ctr - 1].symbol = table[i].symbol;
			table1[ctr - 1].num = table[i].num;
			table2 = table1;
		}
	}
	table = table2;
	int ctr1 = 0;
	for (int i = 0; i < ctr; i++) {
		ctr1 += table[i].num;
	}
	for (int i = 0; i < ctr; i++) {
		table[i].chance = (float)table[i].num / (float)ctr1;
	}
	mas tab;
	tab.table = table;
	tab.ctr = ctr;
	return tab;
}
mas TableSort(mas tab) {
	for (int i = 1; i < tab.ctr; ++i) {
		for (int r = 0; r < tab.ctr - i; r++) {
			if (tab.table[r].chance < tab.table[r + 1].chance) {
				elem temp = tab.table[r];
				tab.table[r] = tab.table[r + 1];
				tab.table[r + 1] = temp;
			}
		}
	}

	return tab;
}
struct TreeNode {
	elem node;
	TreeNode** children = new TreeNode *[2];
	int priority = 1;
	bool checker = 0;
	int code = -1;
};
void BuildTree(TreeNode** &element, int ctr);
TreeNode* BuildQueue(mas tab) {
	int ctr = tab.ctr;
	TreeNode** queue = new TreeNode *[ctr];
	for (int i = 0; i < ctr; i++) {
		TreeNode* a = new TreeNode;
		a->node = tab.table[i];
		a->priority = tab.table[i].num;
		queue[i] = a;
	}
	while (ctr != 1) {
		BuildTree(queue, ctr);
		ctr--;
	}
	return queue[0];
}
void BuildTree(TreeNode** &element, int ctr) {
	TreeNode* temp = new TreeNode;
	TreeNode* temp1 = new TreeNode;
	temp->priority = 0;
	temp1->priority = 0;
	int index;
	int index1;
	for (int i = 0; i < ctr; i++) {
		if (temp->priority != 0) {
			if (element[i]->priority < temp->priority) {
				temp1 = temp;
				index1 = index;
				temp = element[i];
				index = i;
			}
			else if (element[i]->priority < temp1->priority || temp1->priority == 0) {
				temp1 = element[i];
				index1 = i;
			}
		}
		else {
			temp = element[i];
			index = i;
		}
	}
	TreeNode* ret = new TreeNode;
	ret->checker = 1;
	ret->children = new TreeNode *[2];
	ret->children[0] = temp;
	ret->children[1] = temp1;
	ret->priority = temp->priority + temp1->priority;
	TreeNode** element1 = new TreeNode *[ctr - 1];
	element[index] = ret;
	for (int i = 0; i < ctr; i++) {
		if (i < index1) {
			element1[i] = element[i];
		}
		if (i > index1) {
			element1[i - 1] = element[i];
		}
	}
	element = element1;
}
void printTree(TreeNode* tree, int ctr) {

	ctr++;
	if (tree->checker) {
		printTree(tree->children[0], ctr);
	}
	for (int i = 0; i < ctr - 1; i++)
		cout << "\t";
	cout << tree->node.symbol << "(" << tree->code << ")" << endl;
	if (tree->checker) {
		printTree(tree->children[1], ctr);
	}
}
void setIndex(TreeNode* tree) {
	if (tree->checker) {
		tree->children[0]->code = 0;
		tree->children[1]->code = 1;
		setIndex(tree->children[0]);
		setIndex(tree->children[1]);
	}
}
bool endpoint = false;
string getSymbolCode(char symbol, string code, TreeNode* tree) {
	if (tree->node.symbol == symbol) {
		endpoint = true;
		code += to_string(tree->code);
		return code;
	}
	else {
		if (tree->checker) {
			if (!endpoint) {
				code += getSymbolCode(symbol, "", tree->children[0]);
			}
			if (endpoint) {
				code += to_string(tree->code);
				return code;
			}
			if (!endpoint) {
				code += getSymbolCode(symbol, "", tree->children[1]);
			}
			if (endpoint) {
				code += to_string(tree->code);
				return code;
			}
		}
	}
	return"";
}
string stringCode(string name, TreeNode* tree, bool hm) {
	string code = "";
	for (int i = 0; i < name.size(); i++) {
		string codepart = getSymbolCode(name[i], "", tree);
		reverse(codepart.begin(), codepart.end());
		string codepart1 = "";
		for (int j = 2; j < codepart.size(); j++) {
			codepart1 += codepart[j];
		}
		if (hm)
			code += codepart1;
		if (!hm) {
			code += codepart1;
			code += " ";
		}
		endpoint = false;
	}
	return code;
}
string getCorrectSymbolCode(char symbol, TreeNode* tree) {
	string codepart = getSymbolCode(symbol, "", tree);
	reverse(codepart.begin(), codepart.end());
	string codepart1 = "";
	for (int j = 2; j < codepart.size(); j++) {
		codepart1 += codepart[j];
	}
	endpoint = false;
	return codepart1;
}
int main() {
	setlocale(LC_ALL, "rus");
	string a = "bigteststringstr";
	mas tab = Tablein(a);
	cout << "Таблица частот встречаемости символов:" << endl;
	for (int i = 0; i < tab.ctr; i++) {
		cout << i + 1 << ". " << "Символ - " << tab.table[i].symbol << ", количествовхождений - " << tab.table[i].num << ", вероятность - " << tab.table[i].chance << ";" << endl;
	}
	tab = TableSort(tab);
	cout << endl;
	cout << "Отсортированная таблица частот встречаемости символов:" << endl;
	for (int i = 0; i < tab.ctr; i++) {
		cout << i + 1 << ". " << "Символ - " << tab.table[i].symbol << ", количествовхождений - " << tab.table[i].num << ", вероятность - " << tab.table[i].chance << ";" << endl;
	}
	TreeNode* tree = BuildQueue(tab);
	setIndex(tree);
	cout << endl;
	cout << "Дерево Хаффмана (табуляция демонстрирует глубину узла):" << endl;
	printTree(tree->children[0], 1);
	cout << tree->node.symbol << "(" << tree->code << ")" << endl;
	printTree(tree->children[1], 1);
	string LHuff = stringCode(a, tree, 1);
	string LHuff1 = stringCode(a, tree, 0);
	cout << "Закодированная исходная строка:" << endl;
	cout << LHuff1 << endl;
	int Lascii = 8 * a.size();
	int Lhuff = LHuff.size();
	float KSascii = (float)Lascii / (float)Lhuff;
	cout << "Коэффициент сжатия относительно кодировки ASCII: " << KSascii << endl;
	float tmp = log(a.size()) / log(2);
	int tmp1 = ceil(tmp);
	float KSravnom = (float)(tmp1 * a.size()) / (float)Lhuff;
	cout << "Коэффициент сжатия относительно равномерного кода: " << KSravnom << endl;
	float srdlcd = 0;
	string numm = "";
	for (int i = 0; i < tab.ctr; i++) {
		numm = getCorrectSymbolCode(tab.table[i].symbol, tree);
		srdlcd += tab.table[i].chance * float(numm.size());
	}
	cout << "Средняя длина полученного кода: " << srdlcd << endl;
	float temp = 0;
	for (int i = 0; i < tab.ctr; i++) {
		numm = getCorrectSymbolCode(tab.table[i].symbol, tree);
		temp += tab.table[i].chance * pow((float(numm.size()) - srdlcd), 2);
	}
	cout << "Дисперсия: " << temp;
}
