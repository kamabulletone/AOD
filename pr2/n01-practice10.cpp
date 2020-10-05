#include <iostream>
#include <queue>
#include <vector>
using namespace std;

enum Color { RED, BLACK };

struct Node
{
	int data;
	bool color;
	Node* left, *right, *parent;

	Node(int data)
	{
		this->data = data;
		left = NULL;
		right = NULL;
		parent = NULL;
		this->color = RED;
	}
};


class RBTree
{


protected:
	void rotateLeft(Node*&, Node*&);
	void rotateRight(Node*&, Node*&);
	void fixViolation(Node*&, Node*&);
public:
	Node* root;
	RBTree() { root = NULL; }
	void insert(vector<int> n);
	void inorder();
	void levelOrder();
	void remove(vector<int>& a, int data);
};

void inorderHelper(Node* root)
{
	if (root == NULL)
		return;

	inorderHelper(root->left);
	cout << root->data << " ";
	inorderHelper(root->right);
}

Node* BSTInsert(Node* root, Node* pt)
{
	if (root == NULL)
		return pt;

	if (pt->data < root->data)
	{
		root->left = BSTInsert(root->left, pt);
		root->left->parent = root;
	}
	else if (pt->data > root->data)
	{
		root->right = BSTInsert(root->right, pt);
		root->right->parent = root;
	}

	return root;
}



void print_(Node* t, int u) {
	if (t == NULL)
		return;
	else {
		print_(t->left, ++u);
		for (int i = 0; i < u; ++i)
			cout << "|";
		cout << t->data;
		if (t->color)
		{
			cout << " (BLACK)" << endl;
		}
		else
		{
			cout << " (RED)" << endl;
		}
		u--;
		print_(t->right, ++u);
	}
}




void RBTree::rotateLeft(Node*& root, Node*& pt)
{
	Node* pt_right = pt->right;

	pt->right = pt_right->left;

	if (pt->right != NULL)
		pt->right->parent = pt;

	pt_right->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_right;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_right;

	else
		pt->parent->right = pt_right;

	pt_right->left = pt;
	pt->parent = pt_right;
}

void RBTree::rotateRight(Node*& root, Node*& pt)
{
	Node* pt_left = pt->left;

	pt->left = pt_left->right;

	if (pt->left != NULL)
		pt->left->parent = pt;

	pt_left->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_left;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_left;

	else
		pt->parent->right = pt_left;

	pt_left->right = pt;
	pt->parent = pt_left;
}


void RBTree::fixViolation(Node*& root, Node*& pt)
{
	Node* parent_pt = NULL;
	Node* grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != BLACK) &&
		(pt->parent->color == RED))
	{

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		if (parent_pt == grand_parent_pt->left)
		{

			Node* uncle_pt = grand_parent_pt->right;

			if (uncle_pt != NULL && uncle_pt->color == RED)
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}
			else
			{
				if (pt == parent_pt->right)
				{
					rotateLeft(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				rotateRight(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}
		else
		{
			Node* uncle_pt = grand_parent_pt->left;

			if ((uncle_pt != NULL) && (uncle_pt->color == RED))
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}
			else
			{
				if (pt == parent_pt->left)
				{
					rotateRight(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				rotateLeft(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}
	}

	root->color = BLACK;
}

void RBTree::remove(vector<int>& a, int data)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] == data) {
			a.erase(a.begin() + i);
		}
	}
	RBTree tree;
	tree.insert(a);

	tree.inorder();
	cout << endl;

	tree.levelOrder();
	cout << endl;
}

void RBTree::insert(vector<int> data)
{
	for (int i = 0; i < data.size(); i++)
	{
		Node* pt = new Node(data[i]);

		root = BSTInsert(root, pt);

		fixViolation(root, pt);
	}

}

void RBTree::inorder() { inorderHelper(root); }
void RBTree::levelOrder() { print_(root, 0); }


int main()
{
	RBTree tree;

	vector<int> a;
	int vvod;
	cin >> vvod;

	while (vvod != 0)
	{
		a.push_back(vvod);
		cin >> vvod;
	}

	tree.insert(a);


	tree.inorder();
	cout << endl;


	tree.levelOrder();
	cout << endl;

	//tree.remove(a, 9);

	return 0;
}
