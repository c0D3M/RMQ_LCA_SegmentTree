#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
#if 0
class LCA
{
	public:
	/*
	  Convert LCA to RMQ using Euler Tour and then use any of the RMQ method to find LCA.
	  Note that if input tree contain n nodes , Euler tree contains 2n-1 nodes and hence Time complexity
	  of any solution of RMQ should be calculated on 2n-1 nodes.
	*/
	int findLCA(Tree t, int u, int v);
};
#endif
class RMQ
{
	private:
		int buildSegmentTree(vector<int> input, vector<int> &segmentTree, int high, int low, int index);
		int queryRMQ(vector <int> segmentTree, int i, int j, int low, int high, int index);
	public:
		/*
		  Returns index of RMQ on a input vector between i and j
		 */
		int Brute_Force(vector<int> input, int i, int j); // (O(n^2), O(1))
		int Sparse_Table(vector<int> input, int i, int j);// (O( n log (n)), O(1))
		int Segment_Tree(vector<int> input, int i, int j);// (O(n), O(log(n))
		//int Reduced_RMQ(vector<int> input, int i, int j); // (O(n) , O(1)
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
int RMQ::Sparse_Table(vector<int> input, int i, int j)
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
	// Now find 2 block which overlap i,j
	int count = floor(log2 (j-i+1));
	int block2_start = j - (1 << count)+1;
		
#if 0
	cout << "count = "<<count<<endl;
	cout << i<< i+ (1<<count)-1<<endl; 
	cout << block2_start << j<<endl;

	for (int u=0; u < input.size();u++)
	{
		for (int v=0; 1<< v < input.size();v++)
			cout << M[u][v];
		cout<< endl;
	}
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
int RMQ::queryRMQ(vector <int> segmentTree, int i, int j, int low, int high, int index)
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
		int a  = queryRMQ(segmentTree, i, j, low, mid, (2*index)+1); 
		int b = queryRMQ(segmentTree, i, j, mid+1, high, (2*index)+2);
		return (a<b)?a:b;
	}
}
int RMQ::Segment_Tree(vector<int> input, int i, int j)
{
	int size = (pow(2, ceil(log2(input.size())))*2)-1;
	//cout <<size<<endl;
	vector<int> segmentTree(size, INFINITY);
	buildSegmentTree(input, segmentTree, 0, input.size()-1, 0);
	#if 0
	for (int u=0; u < segmentTree.size();u++)
		cout << segmentTree[u]<<" ";
	cout <<endl;
	#endif
	return queryRMQ(segmentTree, i, j, 0, input.size()-1, 0);
}
int main()
{
	RMQ r;
	//vector<int> a = {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	vector<int> a = {-1, 0, 3, 6,-2, 7};
	int i=0, j=4;
	int min = r.Brute_Force(a, i, j);
	cout << "[DP]RMQ index is  "<< min <<" value is "<< a[min]<<endl;
	min = r.Sparse_Table(a, i, j);
	cout << "[Sparse_Table]RMQ index is  "<< min <<" value is "<< a[min]<<endl;
	min = r.Segment_Tree(a, i, j);
	cout << "[Segment_Tree]RMQ value is  "<< min <<endl;
	
	return 0;
}