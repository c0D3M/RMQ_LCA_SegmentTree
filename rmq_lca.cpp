#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
typedef struct _node
{
	int parent;
	vector<int> child; //n-ary Tree
}N;

class RMQ
{
	public:
		RMQ(){};
	    RMQ(int b):B(b){};
		
	    /* Segment Tree */
		vector<int> Segment_Tree(vector<int> input, int i, int j);// (O(n), O(log(n))
		int buildSegmentTree(vector<int> input, vector<int> &segmentTree, int high, int low, int index);
		int  queryRMQ_SegmentTree(vector <int> segmentTree, int i, int j, int low, int high, int index);
		
		/* Sparse Table */
		vector <vector<int>> buildSparseTable(vector<int> input);// (O( n log (n)), O(1))
		int queryRMQ_SparseTable(vector<vector<int>> M, vector<int>, int i, int j);// (O( n log (n)), O(1))
		
		/* Cartesian Tree */
		void displayNodes(vector<int> A, vector<N> T, int C);
		int buildCartesianTree(vector<int> A, vector<N> &T);
		void EulerTour(vector<N> &T, int level, int &count, int index, vector<int> & E, vector<int> & L, vector<int> & R);
	
		/*
		  Returns index of RMQ on a input vector between i and j
		 */
		int Brute_Force(vector<int> input, int i, int j); // (O(n^2), O(1))
		vector<int> E;
		vector<int> L;
		vector<int> Li;
		vector<int> R;
		
		//Reduced RMQ
		int blocksize;
		int blockcount;
		int blockcombination;
		vector<int> M;
		vector <vector<int>> ST;
		vector <int> ST_input;
		vector <int> ST_Index;
		int B; // branching factor i.e. maximum child a node can have
		
		void Reduced_RMQ(vector<int> input); // (O(n) , O(1)
		void buildMappingTable(vector<int> input, vector<N> &T, int root=0);
		int  query_Reduced_RMQ(vector<int>, int x, int y, bool lca=false);
};
int RMQ::Brute_Force(vector<int> input, int i, int j)
{
	vector <vector<int>> M(input.size(), vector<int>(input.size()));
	//Initialize for same indices
	for (int u=0; u < input.size();u++)
		for (int v=0; v < input.size();v++)
			M[u][v] = u;
	
	for (int u=0; u < input.size();u++)
		for (int v=u+1; v < input.size(); v++)
			if(input[M[u][v-1]] < input[v])
				M[u][v] = M[u][v-1];
			else
				M[u][v] = v;
	
	return M[i][j];
}
vector <vector<int>>  RMQ::buildSparseTable(vector<int> input)
{
	vector <vector<int>> M(input.size(), vector<int>(input.size()));
	//Initialize for same indices
	for (int u=0; u < input.size();u++)
		for (int v=0; 1<< v < input.size();v++)
			if(v==0)
				M[u][v] = u;
			else
				M[u][v] = -1;
			
	// 1,2,4, 8 block size
    for(int u=1; (1 << u)  <= input.size();u++)
		for(int v =0; v <= input.size()- (1<<u); v++)
		{
			int end = v + (1<<u) - 1;
			int mid = (v + end )/ 2;
			if(input[M[v][u-1]] <= input[M[mid+1][u-1]])
				M[v][u] = M[v][u-1];
			else
				M[v][u] = M[mid+1][u-1];
		}
			
#ifdef DEBUG
	cout <<" Sparse Table"<<endl;
	cout <<" ------------"<<endl;
	for (int u=0; u < input.size();u++)
	{
		for (int v=0; 1<< v < input.size();v++)
			if(M[u][v]!=-1)
				cout << u<<","<<u+(1<<v)-1<<"->"<<M[u][v]<<endl;
		cout<< endl;
	}
	cout <<endl;
#endif
    return M;
	
}
int RMQ::queryRMQ_SparseTable(vector<vector<int>>M, vector<int> input, int i, int j)
{
	// Now find 2 block which overlap i,j
	int count = floor(log2 (j-i+1));
	int block2_start = j - (1 << count)+1;

	#ifdef DEBUG
	cout << "Range"<<i<<j<<" count = "<<count<<endl;
	cout << "[Block 1]"<< i<< i+ (1<<count)-1<<endl; 
	cout << "[Block 2]" << block2_start << j<<endl;
	#endif

	return (input[M[i][count]]<input[M[block2_start][count]]) ? M[i][count] : M[block2_start][count];	
}
int RMQ::buildSegmentTree(vector<int> input, vector<int> &segmentTree, int low, int high, int index)
{
	if(high==low)
	{
		segmentTree[index] = input[low];
		return segmentTree[index];
	}
	int mid = (low + high ) /2;
	int a = buildSegmentTree(input, segmentTree, low, mid, (2*index )+1);
	int b = buildSegmentTree(input, segmentTree, mid+1, high, (2*index )+2);
	segmentTree [index] = a<b?a:b;
	return segmentTree[index];
}
int RMQ::queryRMQ_SegmentTree(vector <int> segmentTree, int i, int j, int low, int high, int index)
{
	// complete overlap
	if(low>= i && high <=j)
		return segmentTree[index];
	// no overlap
	else if ((high < i) || (low > j))
		return INFINITY;
	// partial overlap
	else
	{
		int mid = (low + high ) /2;
		int a  = queryRMQ_SegmentTree(segmentTree, i, j, low, mid, (2*index)+1); 
		int b = queryRMQ_SegmentTree(segmentTree, i, j, mid+1, high, (2*index)+2);
		return (a<b)?a:b;
	}
}
vector<int> RMQ::Segment_Tree(vector<int> input, int i, int j)
{
	int size = (pow(2, ceil(log2(input.size())))*2)-1;	
	vector<int> segmentTree(size, INFINITY);
	buildSegmentTree(input, segmentTree, 0, input.size()-1, 0);
	#if DEBUG
	cout <<"Size of Array"<<size<<endl;
	cout << "Segment Tree Array"<<endl;
	cout << "------------------"<<endl;
	for (int u=0; u < segmentTree.size();u++)
		cout << segmentTree[u]<<" ";
	cout <<endl<<endl;
	#endif	
	return segmentTree;
}
int RMQ::buildCartesianTree(vector<int> A, vector<N> &T)
{
	int C = A.size();
	int stack[C];
	int tos = -1; // Top of Stack
	for(int i=0; i<C; i++)
	{
		T[i].child.resize(B);// Cartesian Tree is binary !
		int j = tos;
		while((j!=-1) && (A[stack[j]] > A [i]))
			j--;
		
		if(tos==-1)// First entry
		{
			T[i].parent = -1;
			T[i].child[0] =  -1;
			T[i].child[1] = -1;
		}
		else if(j==tos) // tos is smaller than i , just add case
		{
			T[i].parent = stack[tos];
			T[i].child[0] = T[i].child[1] = -1;
			if(T[stack[tos]].child[0]==-1)
				T[stack[tos]].child[0] = i;
			else
				T[stack[tos]].child[1] = i;
		}
		else
		{
			if(j!=-1)
			{
				// Some items removed from stack
				if(T[stack[j]].child[1]==-1)
					T[stack[j]].child[0] = i;
				else
					T[stack[j]].child[1] = i;
				
				T[i].parent = stack[j];
			}
			else
			{
				// Case where entry i is smallest in whole stack (whole stack is emptied)
				T[i].child[1] = -1;
				T[i].parent = -1;
			}
			T[i].child[1] = -1;
			T[i].child[0]  = stack[j+1]; // j+1 ->This is the last one removed
			T[stack[j+1]].parent = i;
		}
			
		stack[++j] = i;
		tos = j;
	}
	return stack[0];
}
void RMQ::displayNodes(vector<int> A, vector<N> T, int C)
{
	cout<<"Index |Value |Parent |Left |Right"<<endl;
	for (int i =0; i< C; i++)
		cout <<i<<"      "<<A[i]<<"      "<<T[i].parent<<"      "<<T[i].child[0]<<"      "<<T[i].child[1]<<endl;
}
void RMQ:: EulerTour(vector<N> &T, int level, int &index, int node, vector<int> & E, vector<int> & L, vector<int> & R)
{
	E[index] = node;
	L[index] = level;
	if(R[node]==-1)
		R[node] = index;
	for (int i=0;i< B; i++)
	{
		if(T[node].child[i] ==-1)
			return;
		EulerTour(T, level+1, ++index, T[node].child[i], E, L, R);
		++index;
		E[index] = node;
		L[index] = level;
	}
	
}
void RMQ::Reduced_RMQ(vector<int> input)
{
	/*
	  We are given an inpur array and we need to find RMQ using Generalized RMQ
	  Step 1) Convert to a Tree using Cartesian Tree this take O(n)
	  Step 2) Do Euler Tour on this tree and generate E, L, R array.
	  Step 3) Modify L to L[i] = L[i] - L[i-1]
	  Step 4) Replace -1 by 0
	  Step 5) Divide this L array in log (n)/2 blocksize , there would be 2n/log(n) such blocks
	  Step 6) Compute and store minium between every pair in each block , their would be (blocksize * (blocksize+1))/2
	  Step 7) Also store minimum of each block in an array 'A' and prcoess it using Sparse Table algorithm , to find the real index we have 
	          to also store minium index in a separate array B.
	   		  
	  Querying:
	  Given i,j
	  Step 1) Find which block i and j lie (i/blocksize)
	  Step 2) Lookup [i,blocksize] in block of i
	  Step 3) Lookup [0, j] in block of j
	  Step 4) Using Sparse Table that we created in step 7 , i+1,j-1 and get index and B[index] will give value in L.
	  
	  
	*/
	//Step 1
	int C = input.size();
	vector<N> T(C);
	B = 2;
	int root = buildCartesianTree(input, T);
	#ifdef DEBUG
	cout<< "In order Traversal"<<endl;
	cout << "-----------------"<< endl;
	displayNodes(input, T, C);
	#endif
	buildMappingTable(input, T, root);
}
void RMQ::buildMappingTable(vector<int> input, vector<N> &T, int root)
{	
	int C = input.size();
	
	//Step 2
	E.resize(2*C -1);
	L.resize(2*C -1);
	R.resize(C, -1);	 
	int c = 0;
	EulerTour(T, 0, c, root, E, L, R);
	//Step 3 & 4
	Li.resize(2*C -1);
	for (int ii =1; ii< L.size(); ii++)
	{
		Li[ii] = L[ii] -  L[ii-1];
	    if(Li[ii]<0) 
			Li[ii] = 0;
	}
	//Step 5
	blocksize = ceil((double)log2(2*C-1))/2;
	blockcount = ceil((double)(2*C -1) / (double)blocksize);
	blockcombination = blocksize * (blocksize+1)/2;
	#ifdef DEBUG
	cout<< "input "<<C<<" blocksize "<< blocksize<<" blockcount "<<blockcount<<" blockcombination "<< blockcombination<<endl;
	#endif
	//Step 6
	M.resize(blockcombination * blockcount);
	for(int ii =0; ii<blockcount; ii++)
	{
		int startIndex =  ii * blockcombination;
		int rowIndex = 0;
		for(int jj=0;jj < blocksize; jj++)
		{
			int min = Li[ii*blocksize];
			for(int k=jj; k < blocksize ;k++)
				if(Li[(ii*blocksize)+k] == min)
				{
					if(min==1)
						//Take left one 1->1 (going downward)
						M[startIndex+(rowIndex++)] = jj;
					else
						// Take right one 0->0 (going upward )
						M[startIndex+(rowIndex++)] = k;
				}			
				else if (Li[(ii*blocksize)+k] < min)
					M[startIndex+(rowIndex++)] = k;
				else
					M[startIndex+(rowIndex++)] = jj;
		}
	}
	// Step 7
	// Use Sparse Table for finidng minumium across block
	ST.resize(blockcount, vector<int>(blockcount));
	ST_input.resize(blockcount);
	ST_Index.resize(blockcount);
	for(int ii =0; ii<blockcount; ii++)
	{
		// Get (0,blocksize) in each block , that will stored at this location
		int idx = M[blocksize -1 + ii*blockcombination];
		// Convert that to absolute index
		idx = (ii * blocksize) + idx;
		// Look this up in E to get array index
		ST_input[ii] = input[E[idx]];
		ST_Index [ii] = idx;
	}
	ST = buildSparseTable(ST_input);
	return;
}	
int RMQ::query_Reduced_RMQ(vector<int> input, int x, int y, bool lca)
{
	// ST_start = 7/5 + 1 =2
	// ST_end = 23/5 -1 = 3
	// inblock_start = 7%5 =2 ->  block 1 ->[2, blocksize] -> index = 1* blockcombination + (blockcombination - 2*3/2 -1)
	// inblock_end = 23%5 = 3 ->  block 4 -> [0,3]  -> index = 4* blockcombination + inblock_end
	// 10, 19  and 10,20
	int i = R[x];
	int j=  R[y];
	
	int ST_start = i/blocksize;
	int ST_end =  j/blocksize;
	int inblock_start = i%blocksize;
	int inblock_end = (j%blocksize); 
	int inblock_start_index= -1, inblock_end_index = -1;
	#ifdef DEBUG
		cout<<"i="<<i<<" j="<<j<<endl;
	#endif
	// Same block test case 5,7
	if(ST_start ==ST_end)
	{
		int left = (blocksize) -  inblock_start;
		inblock_start_index = (ST_start) * blockcombination + (blockcombination - ((left * (left+1))/2) ) + inblock_end;
	}
	else
	{
		// Next block test case 2, 6
		// boundary case 9,19 ; 9,20;10,19; 10,20;
		
		// Border case i.e. i is multiple of blockcount e.g. 0,5,10,15
		
		if(inblock_start)
		{
			// Finding index of (inblock_start,0) is like finding SUM of Arithemtic Progression series is 5,4,3,2,1 
			// Because the combination are stored like this so if one has to find index of (3,0) it would be 11
			// Agnerelaized AP sum forumal for this is 
			// n/2 (2a+ (n-1)d   ; a= 5, d= -1 , since n is counted from 0 , replace n-> n+1
			// (n+1)/2[2a+ nd] , Subtract final answer -1 becuase we count from 0
			
			int left = ((inblock_start+1)*(2*blocksize - inblock_start))/2;
			inblock_start_index = ((ST_start) * blockcombination) +  left -1;
			ST_start ++; 
		}
		//Border case i.e. j is 4,9,14,19 boundary of blocksize
		if(inblock_end!=blocksize-1)
		{
			inblock_end_index = ((ST_end * blockcombination) -1) + inblock_end +1; //+1/-1 because we are counting from 0
			ST_end --;
		}
	}
	int firstBlockMinimum =  (inblock_start_index!=-1)?E[M[inblock_start_index]+((i/blocksize)*blocksize)]:-1;
	int lastBlockMinimum = (inblock_end_index!=-1)?E[M[inblock_end_index]+((j/blocksize)*blocksize)]:-1;
	int stBlockMinimum = (ST_end>=ST_start)? E[ST_Index[queryRMQ_SparseTable(ST, ST_input, ST_start, ST_end)]]:-1;
	
	#ifdef DEBUG
	cout<<"ST_start "<<ST_start<<" ST_end "<<ST_end<<" inblock_start_index "<<inblock_start_index<<" inblock_end_index "<<inblock_end_index<<endl;
	
	cout <<"Start Index "<<firstBlockMinimum<<endl;
	cout <<"End Index "<<lastBlockMinimum<<endl;
	cout <<"ST Index "<<stBlockMinimum<<endl;
	#endif
	if(lca)
	{
		int minimum = firstBlockMinimum==-1? INFINITY: firstBlockMinimum;
		if(lastBlockMinimum!=-1 &&  lastBlockMinimum <= minimum)
		{
			minimum = lastBlockMinimum;
		}
		if(stBlockMinimum!=-1 &&  stBlockMinimum <= minimum)
		{
			minimum = stBlockMinimum;				
		}
		return minimum;
	}
	else
	{
		//RMQ needs an extra level  of indexing because value is E are cartesian tree (which is array index not real value)
		int minimum = firstBlockMinimum==-1? INFINITY: input[firstBlockMinimum];
		int minimum_index = firstBlockMinimum;
		if(lastBlockMinimum!=-1 &&  input[lastBlockMinimum] <= minimum)
		{
			minimum = input[lastBlockMinimum];
			minimum_index = lastBlockMinimum;	
		}
		if(stBlockMinimum!=-1 &&  input[stBlockMinimum] <= minimum)
		{
			minimum = input[stBlockMinimum];
			minimum_index = stBlockMinimum;	
		}
		return minimum_index;
	}
	
}

int main()
{
	#if 0
	RMQ r;
	vector<int> a = {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	//vector<int> a = {-1, 0, 3, 6,-2, 7};
	int i=3, j=5;
	
	//Dynamic Programming
	int min = r.Brute_Force(a, i, j);
	cout << "[DP] RMQ index is  "<< min <<" value is "<< a[min]<<endl;
	
	//Sparse Table 
	vector<vector<int>> sparseTable = r.buildSparseTable(a);
	min =r.queryRMQ_SparseTable(sparseTable, a, i, j);
	cout << "[Sparse_Table] RMQ index is  "<< min <<" value is "<< a[min]<<endl;
	
	//Segment Tree
	vector<int> segmentTree = r.Segment_Tree(a, i, j);
	cout << "[Segment_Tree] RMQ value is  "<< r.queryRMQ_SegmentTree(segmentTree, i, j, 0, a.size()-1, 0) <<endl;
	
	// Generalized RMQ
	r.Reduced_RMQ(a);
	min = r.query_Reduced_RMQ(a, i, j);
	cout << "[Generalized RMQ] RMQ value is  "<< min<<" value is " <<a[min]<<endl;
	#else
	//LCA
	int b;
	cin>> b;
	int n;
	cin >> n; 
	vector<N> nodes(n);
	vector<int> input(n);
	for(int ii = 0; ii<n;ii++)
	{
	int count =0;	
	int t;
	input[ii] = ii;
	do{
		
		cin >> t;
		nodes[ii].child.push_back(t);
		}while(t!=-1);
	}

	RMQ r(b);
	
	r.buildMappingTable(input, nodes, 0);
	int min = r.query_Reduced_RMQ(input, 1, 1, true);
	cout <<" LCA = "<< min;
	#endif
	
	return 0;
}