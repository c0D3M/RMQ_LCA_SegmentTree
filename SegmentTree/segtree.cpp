#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
// Define this based on problem type , for RMQ this should be a big value, for problems sum of range this would be 0
#define NO_OVERLAP 5000
class SegmentTree
{
	private:
		vector<int> input;
		vector<int> segmentTree; // SegTree Array
		vector<int> lazy; 		 // Lazy	Array
		int size; // Size of SegTree
     	buildSegmentTree(int low, int high, int index);
		int operation(int a, int b);// Based on problem type modify this function
	public:
		SegmentTree(vector<int> a);
		int query(int i, int j, int low, int high, int index);
		void updateRange(int idx, int value, int low, int high, int index);
		void updateRangeMultiple(int value, int seg_start, int seg_end, int low, int high, int index);
		void updateRangeLazy(int value, int seg_start, int seg_end, int low, int high, int index);
		int queryLazy(int i, int j, int low, int high, int index);
};
SegmentTree::SegmentTree(vector<int> a)
{
	input = a;
	// For an input array of size 'n' , segment tree would contain 2n-1 node
	// At each level , nodes double up i.e 1, 2, 4, 8 and there would be maximum log n height
	// Use Geometric progression = n-1 + n leaf nodes  = 2n-1
	int x = (int)(ceil(log2(input.size()))); 
 
    //Maximum size of segment tree
    int max_size = 2*(int)pow(2, x) - 1; 
	
	size = max_size;
	segmentTree.reserve(size);
	lazy.reserve(size);
	buildSegmentTree(0, a.size()-1, 0);
}
int SegmentTree::operation(int a, int b)
{
	// For minimum Query, modify according to problem statement
	//return a+b;
	return a<b?a:b;
}
/*
  Recursively build Segment Tree on left and right half
  parent contain minimum of left and right half
  @low: low 
  @high: high
  @index: current index to use for the result
*/
int SegmentTree::buildSegmentTree(int low, int high, int index)
{
	if(high==low)
	{
		segmentTree[index] = input[low];
		return segmentTree[index];
	}
	
	/* Build array to store implicit segment tree
	   SegTree are balanced so can be stored implicit 
	   Left child = 2i+1 
	   Right Child = 2i+2
	   Parent = (i-1)/2
	*/
	int mid = (low + high ) /2;
	int a = buildSegmentTree(low, mid, (2*index )+1);
	int b = buildSegmentTree(mid+1, high, (2*index )+2);
	segmentTree [index] = operation(a, b);
	return segmentTree[index];
}
/*
@segmentTree : segment Tree 
@i : low range 
@ j : high range
@ low: subtree low
@ high: subtree high
@ index: index in segmentTree array, Each Node will contain result of input array [low, high] operation
*/
int SegmentTree::query(int i, int j, int low=0, int high=0, int index=0)
{
	//Trick to detect first call and update high
	if(!low && !high && !index)
		 high = input.size()-1;
	// complete overlap
	if(low>= i && high <=j)
		return segmentTree[index];
	// no overlap
	else if ((high < i) || (low > j))
		return NO_OVERLAP;
	// partial overlap
	else
	{
		int mid = (low + high ) /2;
		int a = query(i, j, low, mid, (2*index)+1); 
		int b = query(i, j, mid+1, high, (2*index)+2);
		return operation(a, b);
	}
}
/*
 @i :   low index for updateRange
 @j :   high index for updateRange
 @value:value to be updated 
 @low:  low index for SegTree
 @high: high index for SegTree
 @index: for saving
 */
void SegmentTree::updateRange(int idx, int value, int low=-1, int high=-1, int index=0)
{
	if( (low==-1) && (high==-1)) //TRick to detect first call
		{low =0 ; high = input.size()-1;}
	if(low == idx)
		segmentTree[low] += value;
	else if(low<= idx && idx <=high)
	{
		segmentTree[index] +=  value;
		int mid = (low + high ) /2;
		updateRange(idx, value, low, mid, (2*index)+1); 
		updateRange(idx, value, mid+1, high, (2*index)+2);
	}
	else 
		return ;
}
void SegmentTree::updateRangeMultiple(int value, int low, int high,int seg_start=-1, int seg_end=-1, int index =0)
{
	if( (seg_start==-1) && (seg_end==-1)) //TRick to detect first call
		{seg_start =0 ; seg_end = input.size()-1;}
		
	if((seg_end < low) || (seg_start > high) || (seg_start> seg_end))//Out of range
		return;
	if(seg_start ==seg_end)
	{
		segmentTree[index] += value;
		return;
	}
	int mid = (seg_start + seg_end)/2;
	updateRangeMultiple(value, low, high, seg_start, mid, 2*index+1);
	updateRangeMultiple(value, low, high, mid+1, seg_end, 2*index+2);
	//Now update the segmentTree
	segmentTree[index] = segmentTree[2*index+1] + segmentTree[2*index+2];
	
}
void SegmentTree::updateRangeLazy(int value, int low, int high,int seg_start=-1, int seg_end=-1, int index =0)
{
	if( (seg_start==-1) && (seg_end==-1)) //TRick to detect first call
		{seg_start =0 ; seg_end = input.size()-1;}
		
	if( !((seg_start <=low) && (high <= seg_end))) // Is there an overlap, if not just return ?
		return ;
		
	// check in lazy array, if non-zero, add that value and push the value to child nodes
	if(lazy[index])
	{
		segmentTree[index] += value; //Update and also push to child nodes
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] = value;
			lazy[2*index+2] = value;
		}
		lazy[index] =0;
	}
	
	if((seg_start > seg_end) || (seg_end < low) || (seg_start > high))
		return;
	// if there is a `complete overlap` stop and update the segmentTree index return
	// complete overlap i.e. segment start & end should fit in the low & high range
	if(seg_start >= low && seg_end<= high)
	{
		segmentTree[index] += value;
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] = value;
			lazy[2*index+2] = value;
		}
		return;
	}
	
	
	// recur further only if the given range partially overlap in segment range.
	int mid = (seg_start + seg_end ) /2;
	updateRangeLazy(value, low, high, seg_start, mid, 2*index+1);
	updateRangeLazy(value, low, high, mid+1, seg_end, 2*index+2);
	segmentTree[index] = operation(segmentTree[2*index+1], segmentTree[2*index+2]);
}
int SegmentTree::queryLazy(int low, int high, int seg_start=0, int seg_end=0, int index=0)
{
	//Trick to detect first call and update high
	if(!seg_start && !seg_end && !index)
		 seg_end = input.size()-1;
	if(lazy[index])
	{
		segmentTree[index] += lazy[index]; //Update and also push to child nodes
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] = lazy[index];
			lazy[2*index+2] = lazy[index];
		}
		lazy[index] =0;
	}
	// complete overlap i.e. segment start & end should fit in the low & high range
	if(seg_start>= low && seg_end <=high)
		return segmentTree[index];
	// no overlap
	else if ((seg_end < low) || (seg_start > high))
		return NO_OVERLAP;
	// partial overlap
	else
	{
		int mid = (seg_start + seg_end ) /2;
		int a = queryLazy(low, high, seg_start, mid, (2*index)+1); 
		int b = queryLazy(low, high, mid+1, seg_end, (2*index)+2);
		return operation(a, b);
	}
}
int main()
{
	//vector<int> a =   {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	vector<int> a = {9, 8, 7, -1, 5, 4, 3, 2};
	int i=0, j=9; // RMQ query between index
	
	class SegmentTree s(a);	
	cout << "[Segment_Tree] RMQ value is  "<< s.queryLazy(i, j) <<endl;
	s.updateRangeLazy(-5, 0, 3);
	cout << "[Segment_Tree] RMQ value is  "<< s.queryLazy(2, 2) <<endl;
#if 0
	cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	i=7; j=9;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	s.updateRange(2, 10);
	cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	i=0; j=3;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	s.updateRangeMultiple(2, 5,9);
	i=5; j=9;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
#endif

}