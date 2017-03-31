## TREAPS ( Tree + Heaps)
Treaps is a randomized data structure which has both the property of binary search tree and max/min heap.<br />
Each node in this tree has a pair __(X,Y)__, Binary search tree property gets satiisfied by X i.e. all __left subtree elements < X __
and right subtree elelments > X.
Also for Y all nodes in left and right subtree are less than Y.Y values are called **priority**.
This data structure is often useful in balancing BST. If priorities are chosen randomly height of this binary search tree is **O(log N)** 

As per Insert procedure , key point is anything you insert left subtree contain node < F and right subtree > F
- P(F) < P(J) we cannot insert here, since K(F) < K(J), goto left subtree.
- P(F) > P(C) , so we know this is the right place of F , call split ( C, F, F->L, F->R).
- K(F) > K(C) so we know that **F->L = C** but we still have to find F->R , which would be __first maximumum__ found in **right subtree**.
  split(C->R, F, C->R, R)
- K(G) > K(F), **first maximum** found , this right subtree of **F->R = G**, now **C->R which was earlier G**, is changed, so find **first minimum** in **left subtree** i.e. G->L split(G->L, F, L, G->L)
- K(D)< K(F), we found first minimum and this will be added to **C->R = D** , once again **G->L** changed , so find first maximum in right subtree.


So recurively this process repeats until we reach empty nodes.
  
