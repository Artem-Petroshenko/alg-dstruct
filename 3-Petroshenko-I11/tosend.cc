//File for testing system

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct tree_t {
	struct tree_t* parent;
	struct tree_t* left;
	struct tree_t* mid;
	struct tree_t* right;
	struct tree_t* extraChild;
	int key1, key2;
	int length;
} tree_t;

tree_t* InitNode(int k);
tree_t* SearchNode(tree_t* root, int x);
int MaxKey(tree_t* t);
void Swap(tree_t** t1, tree_t** t2);
tree_t* SortSons(tree_t* t);
tree_t* UpdateKeys(tree_t* t);
tree_t* SplitParent(tree_t* t);
tree_t* Insert(tree_t* root, int x);
void TreeDestroy(tree_t* root);
tree_t* Remove(tree_t* root, int x);
void PrintTree(tree_t* t, int n);
int Solution(FILE* streamIn, FILE* streamOut);

int main(void) {
	Solution(stdin, stdout);
	return 0;
}

tree_t* InitNode(int k) {
	tree_t* newNode = (tree_t*)malloc(sizeof(tree_t));
	if (!newNode)
		return NULL;
	newNode->key1 = k;
	newNode->key2 = -1;
	newNode->left = NULL;
	newNode->mid = NULL;
	newNode->right = NULL;
	newNode->extraChild = NULL;
	newNode->parent = NULL;
	newNode->length = 0;
	return newNode;
}

tree_t* SearchNode(tree_t* root, int x) {
	if (!root)
		return root;
	tree_t* t = root;
	while (t->length) {
		if (t->length == 2)
			if (t->key1 < x)
				t = t->right;
			else
				t = t->left;
		else if (t->key2 < x)
			t = t->right;
		else if (t->key1 < x)
			t = t->mid;
		else
			t = t->left;
	}
	return t;
}

int MaxKey(tree_t* t) {
	tree_t* a = t;
	while (a->length)
		a = a->right;
	return a->key1;
}

void Swap(tree_t** t1, tree_t** t2) {
	tree_t* tmp = *t1;
	*t1 = *t2;
	*t2 = tmp;
	return;
}

tree_t* SortSons(tree_t* t) {
	if (t->length == 2)
		if (t->right->key1 < t->left->key1)
			Swap(&t->right, &t->left);
	if (t->length == 3) {
		if (t->right->key1 < t->mid->key1)
			Swap(&t->right, &t->mid);
		if (t->mid->key1 < t->left->key1)
			Swap(&t->mid, &t->left);
		if (t->right->key1 < t->mid->key1)
			Swap(&t->right, &t->mid);
	}
	return t;
}

tree_t* UpdateKeys(tree_t* t) {
	tree_t* a = t->parent;
	while (a) {
		a->key1 = MaxKey(a->left);
		if (a->mid)
			a->key2 = MaxKey(a->mid);
		else
			a->key2 = -1;
		a = a->parent;
	}
	return t->parent;
}

tree_t* SplitParent(tree_t* t) {
	if (t->length == 3) {
		t->mid = t->extraChild;
		t->extraChild = NULL;
		t = SortSons(t);
		return t;
	}
	if (t->length == 4) {
		tree_t* a = InitNode(t->right->key1);
		a->parent = t->parent;
		a->right = t->right;
		t->right->parent = a;
		if (t->extraChild->key1 > t->mid->key1) {
			a->left = t->extraChild;
			t->right = t->mid;
		}
		else {
			a->left = t->mid;
			t->right = t->extraChild;
			t->extraChild->parent = t;
		}
		a->left->parent = a;
		t->extraChild = NULL;
		t->mid = NULL;
		a->length = 2;
		t->length = 2;
		a = SortSons(a);
		t = SortSons(t);
		a = UpdateKeys(a->left);
		t = UpdateKeys(t->left);

		if (t->parent) {
			t->parent->extraChild = a;
			t->parent->length++;
			t->parent = SplitParent(t->parent);
			return t;
		}
		else {
			tree_t* newRoot = InitNode(-1);
			if (!newRoot) {
				free(a);
				return t;
			}
			newRoot->left = t;
			newRoot->right = a;
			t->parent = newRoot;
			a->parent = newRoot;
			newRoot->length = 2;
			return newRoot;
		}
	}
}

tree_t* Insert(tree_t* root, int x) {
	tree_t* newNode = InitNode(x);
	if (!newNode)
		return root;
	if (!root) {
		root = newNode;
		return root;
	}
	tree_t* a = SearchNode(root, x);
	if (a->key1 == x)
	{
		free(newNode);
		return root;
	}
	if (!a->parent) {
		tree_t* t = InitNode(-1);
		if (!t) {
			free(newNode);
			return root;
		}
		t->right = a;
		t->left = newNode;
		a->parent = t;
		newNode->parent = t;
		t->length = 2;
		t = SortSons(t);
		newNode = UpdateKeys(newNode);
		return t;
	}
	else {
		tree_t* p = a->parent;
		p->extraChild = newNode;
		newNode->parent = p;
		p->length++;
		p = SplitParent(p);
		newNode = UpdateKeys(newNode);
		if (!p->parent)
			return p;
		else
			return root;
	}
}

void TreeDestroy(tree_t* root) {
	if (root) {
		TreeDestroy(root->left);
		TreeDestroy(root->mid);
		TreeDestroy(root->right);
		TreeDestroy(root->extraChild);
		free(root);
	}
	return;
}

tree_t* Remove(tree_t* root, int x) {
	tree_t* t = SearchNode(root, x);
	if (!t || t->key1 != x)
		return root;
	tree_t* p = t->parent;
	if (!p) {
		TreeDestroy(t);
		return root;
	}
	if (p->length == 3) {
		if (t == p->left) {
			free(t);
			p->left = p->mid;
		}
		else if (t == p->right) {
			free(t);
			p->right = p->mid;
		}
		p->mid = NULL;
		p->length--;
		return root;
	}
	else {
		tree_t* b = t == p->left ? p->right : p->left;
		if (!p->parent) {
			root = b;
			root->parent = NULL;
			free(t);
			free(p);
			return root;
		}
		else {
			tree_t* gp = p->parent;
			if (gp->length == 2) {
				tree_t* np = p == gp->left ? gp->right : gp->left;
				np->extraChild = b;
				np->length++;
				b->parent = np;
				free(t);
				np = SplitParent(np);
				if (np->length == 3) {
					if (!gp->parent) {
						root = np;
						np->parent = NULL;
					}
					else {
						if (gp == gp->parent->left)
							gp->parent->left = np;
						else
							gp->parent->right = np;
						np->parent = gp->parent;
					}
					free(p);
				}
				else {
					if (p == gp->left)
						gp->left = gp->mid;
					if (p == gp->right)
						gp->right = gp->mid;
					gp->mid = NULL;
					free(p);
					gp->length--;
				}
				np = UpdateKeys(b);
			}
			else {
				//in case p is a mid child we take np as the best of left and right, in case p is a right or left child we take np as a mid
				tree_t* np = p == gp->mid ? (gp->left->length < gp->right->length ? gp->left : gp->right) : gp->mid;
				np->extraChild = b;
				np->length++;
				b->parent = np;
				free(t);
				if (p == gp->left)
					gp->left = gp->mid;
				if (p == gp->right)
					gp->right = gp->mid;
				free(p);
				gp->length--;
				np = SplitParent(np);
				b = UpdateKeys(b);
			}
		}
	}
	return root;
}

void PrintTree(tree_t* t, int n) {
	if (!t) {
		return;
	}
	else {
		PrintTree(t->right, n + 2);
		if (t->length == 3) {
			for (int i = 0; i < n; i++) {
				printf(" ");
			}
			printf("%d\n", t->key2);
			PrintTree(t->mid, n + 2);
		}
		for (int i = 0; i < n; i++) {
			printf(" ");
		}
		printf("%d\n", t->key1);
		PrintTree(t->left, n + 2);
	}
}

int Solution(FILE* streamIn, FILE* streamOut) {
	char lineBuffer[16] = " ";
	char action;
	int number;
	tree_t* t = NULL;
	while (fgets(lineBuffer, 16, streamIn)) {
		sscanf(lineBuffer, "%c%i", &action, &number);
		switch (action) {
		case 'a':
			t = Insert(t, number);
			break;
		case 'r':
			t = Remove(t, number);
			break;
		case 'f':
			if (SearchNode(t, number) && SearchNode(t, number)->key1 == number)
				fprintf(streamOut, "yes\n");
			else
				fprintf(streamOut, "no\n");
			break;
			/*case 'p':
				PrintTree(t, 0);
				break;*/
		default:
			TreeDestroy(t);
			return 0;
		}
	}
	TreeDestroy(t);
	return 0;
}