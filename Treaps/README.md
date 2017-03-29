## TREAPS ( Tree + Heaps)
Treaps is a randomized data structure which has both the property of binary search tree and max/min heap.<br />
Each node in this tree has a pair __(X,Y)__, Binary search tree property gets satiisfied by X i.e. all __left subtree elements < X __
and right subtree elelments > X.
Also for Y all nodes in left and right subtree are less than Y.Y values are called **priority**.
This data structure is often useful in balancing BST. If priorities are chosen randomly height of this binary search tree is **O(log N)** 
