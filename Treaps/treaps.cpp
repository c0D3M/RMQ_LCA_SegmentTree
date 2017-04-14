#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
typedef struct treaps
{
	int x;
	int y;
	struct treaps *left;
	struct treaps *right;
}T;
T merge(T* t, T* left, T* right)
{
	if( lefft == NULL) return right;
	if (right == NULL) return left;

	if( left.y > right.y)
		// Left will be new root node , keys of Left.left will always be in left subtree but on right side either Left.Right or Right , recursively break the tie
		{merge(left.right, right); t = left;}
	else 
		// Right will be new root , right.right there will be keys lesser than Right or Left subtree so break ties
		{merge(left, right.left); t = right;}
		
}
void erase(T *t, T* item)
{
	if(t->x==item->x)
		merge(t, t->left, t->right);// We found the node to delete
	else
		erase((item->x < t->x )? t->left:t->right, item); // b-search the key 
}
void split(T* t, T* left, T* right, int key)
{
	if(!t)
		left = right = NULL;
	else if(key > t->key)
		split(t->right, t->right, right, key); left = t;
	else
		split(t->left, left, t->left, key); right = t;
}
void insert(T*t, T*item)
{
	if(item->y > T->y) //We found where to insert , split tree here
		{split(T, item->left, item->right, item);T=item;}
	else //bsearch and go left or right based on key and recurse
		insert((item->x < t->x )? t->left:t->right, item);
}
int main()
{
	return 0;
}