#include <iostream>
using namespace std;
typedef struct _node
{
	int parent;
	int left;
	int right;
}N;

void inOrderTravesal(int A[], N T[] , int index)
{
	if(index == -1)
		return;
	inOrderTravesal(A, T, T[index].left);
	cout << index <<"-->"<< A[index] << " "<<endl;
	inOrderTravesal(A, T, T[index].right);
}
void displayNodes(int A[], N T[], int C)
{
	cout<<"Index |Value |Parent |Left |Right"<<endl;
	for (int i =0; i< C; i++)
		cout <<i<<"      "<<A[i]<<"      "<<T[i].parent<<"      "<<T[i].left<<"      "<<T[i].right<<endl;
}
int main()
{
	int A [] = {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	//int A [] = {1, 2, 3, 4 ,5, 6, 7, 8, 9, 10};
	//int A [] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int C = sizeof(A)/sizeof(int);
	N T[C]; int stack[C];
	int tos = -1; // Top of Stack
	for(int i=0; i<C; i++)
	{
		int j = tos;
		while((j!=-1) && (A[stack[j]] > A [i]))
			j--;
		
		if(tos==-1)
		{
			T[i].parent = -1;
			T[i].left =  -1;
			T[i].right = -1;
		}
		else if(j==tos) // tos is smaller than i , just add case
		{
			T[i].parent = stack[tos];
			T[i].left = T[i].right = -1;
			T[stack[tos]].right = i;			
		}
		else
		{
			if(j!=-1)
			{
				T[stack[j]].right = i;
				T[i].parent = stack[j];
			}
			else
			{
				// Case where entry i is smallest in whole stack (whole stack is emptied)
				T[i].right = -1;
				T[i].parent = -1;
			}
			T[i].right = -1;
			T[i].left  = stack[j+1]; // j+1 ->This is the last one removed
			T[stack[j+1]].parent = i;
		}
		
		stack[++j] = i;
		tos = j;
	}
	cout << "In-Order Traversal of Cartesian Tree"<<endl;
	cout << "------------------------------------"<<endl;
	inOrderTravesal(A, T, stack[0]);
	//displayNodes(A, T, C);
	return 0;
}