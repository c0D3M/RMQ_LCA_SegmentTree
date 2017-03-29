#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
/*
  Recursively build Segment Tree on left and right half
  parent contain minimum of left and right half
*/
int buildSegmentTree(vector<int> input, vector<int> &segmentTree, int low, int high, int index)
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
/*
@segmentTree : segment Tree 
@i : low range 
@ j : high range
@ low: subtree low
@ high: subtree high
@ index: index in segmentTree array
*/
int queryRMQ_SegmentTree(vector <int> segmentTree, int i, int j, int low, int high, int index)
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

int main()
{
	vector<int> a = {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	//vector<int> a = {-1, 0, 3, 6,-2, 7};
	int i=3, j=5;
	
	/* Build array to store implicit segment tree
	   Seg Tree are balanced so can be stored implicit 
	   Left child = 2i+1 
	   Right Child = 2i+2
	   Parent = (i-1)/2
	*/
	int size = (pow(2, ceil(log2(a.size())))*2)-1;	
	vector<int> segmentTree(size, INFINITY);
	buildSegmentTree(a, segmentTree, 0, a.size()-1, 0);
	cout << "[Segment_Tree] RMQ value is  "<< queryRMQ_SegmentTree(segmentTree, i, j, 0, a.size()-1, 0) <<endl;
	
	
}