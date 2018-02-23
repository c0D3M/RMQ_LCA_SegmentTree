#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
using namespace std;
typedef long long ull;
// Define this based on problem type , for RMQ this should be a big value, for problems sum of range this would be 0
#define NO_OVERLAP 0
class SegmentTree
{
	private:
		vector<ull> input;
		ull * segmentTree; // SegTree Array
		ull * lazy; 		 // Lazy	Array
		ull size; // Size of SegTree
     	ull buildSegmentTree(ull low, ull high, ull index);
		ull operation(ull a, ull b);// Based on problem type modify this function
	public:
		
		SegmentTree(vector<ull> a);
		~SegmentTree();
		ull query(ull i, ull j, ull low, ull high, ull index);
		void updateRange(ull idx, ull value, ull low, ull high, ull index);
		void updateRangeMultiple(ull value, ull seg_start, ull seg_end, ull low, ull high, ull index);
		void updateRangeLazy(ull value, ull seg_start, ull seg_end, ull low, ull high, ull index);
		ull queryLazy(ull i, ull j, ull low, ull high, ull index);
		void debug();
};
SegmentTree::SegmentTree(vector<ull> a)
{
	input = a;
	// For an input array of size 'n' , segment tree would contain 2n-1 node
	// At each level , nodes double up i.e 1, 2, 4, 8 and there would be maximum log n height
	// Use Geometric progression = n-1 + n leaf nodes  = 2n-1
	ull x = (ull)(ceil(log2(input.size()))); 
 
    //Maximum size of segment tree
    ull max_size = 2*(ull)pow(2, x) - 1; 
	//cout << max_size<<endl;
	size = max_size;
	segmentTree = new ull[size];
	lazy = new ull[size];
	memset(segmentTree, 0, size*sizeof(ull));
	memset(lazy, 0, size*sizeof(ull));
	//buildSegmentTree(0, a.size()-1, 0);
}
SegmentTree::~SegmentTree()
{
	delete[] segmentTree;
	delete[] lazy;
}
ull SegmentTree::operation(ull a, ull b)
{
	// For minimum Query, modify according to problem statement
	return a+b;
	//return a<b?a:b;
}
/*
  Recursively build Segment Tree on left and right half
  parent contain minimum of left and right half
  @low: low 
  @high: high
  @index: current index to use for the result
*/
ull SegmentTree::buildSegmentTree(ull low, ull high, ull index)
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
	ull mid = (low + high ) /2;
	ull a = buildSegmentTree(low, mid, (2*index )+1);
	ull b = buildSegmentTree(mid+1, high, (2*index )+2);
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
ull SegmentTree::query(ull i, ull j, ull low=0, ull high=0, ull index=0)
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
		ull mid = (low + high ) /2;
		ull a = query(i, j, low, mid, (2*index)+1); 
		ull b = query(i, j, mid+1, high, (2*index)+2);
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
void SegmentTree::updateRange(ull idx, ull value, ull low=-1, ull high=-1, ull index=0)
{
	if( (low==-1) && (high==-1)) //TRick to detect first call
		{low =0 ; high = input.size()-1;}
	if(low == idx)
		segmentTree[low] += value;
	else if(low<= idx && idx <=high)
	{
		segmentTree[index] +=  value;
		ull mid = (low + high ) /2;
		updateRange(idx, value, low, mid, (2*index)+1); 
		updateRange(idx, value, mid+1, high, (2*index)+2);
	}
	else 
		return ;
}
void SegmentTree::updateRangeMultiple(ull value, ull low, ull high,ull seg_start=-1, ull seg_end=-1, ull index =0)
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
	ull mid = (seg_start + seg_end)/2;
	updateRangeMultiple(value, low, high, seg_start, mid, 2*index+1);
	updateRangeMultiple(value, low, high, mid+1, seg_end, 2*index+2);
	//Now update the segmentTree
	segmentTree[index] = segmentTree[2*index+1] + segmentTree[2*index+2];
	
}
void SegmentTree::updateRangeLazy(ull value, ull low, ull high,ull seg_start=-1, ull seg_end=-1, ull index =0)
{
	if( (seg_start==-1) && (seg_end==-1)) //TRick to detect first call
		{seg_start =0 ; seg_end = input.size()-1;}
	
	//cout << "seg_start ="<<seg_start << " seg_end"<<seg_end<<endl;	
		
	// check in lazy array, if non-zero, add that value and push the value to child nodes
	if(lazy[index])
	{
		segmentTree[index] += ((seg_end-seg_start+1)*lazy[index]); //Update and also push to child nodes
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] += lazy[index];
			lazy[2*index+2] += lazy[index];
		}
		lazy[index] =0;
	}
	
	if((seg_start > seg_end) || (seg_end < low) || (seg_start > high))
		return;
	// if there is a `complete overlap` stop and update the segmentTree index return
	// complete overlap i.e. segment start & end should fit in the low & high range
	if(seg_start >= low && seg_end<= high)
	{
		segmentTree[index] += ((seg_end-seg_start+1)*value);
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] += value;
			lazy[2*index+2] += value;
		}
		return;
	}
	
	
	// recur further only if the given range partially overlap in segment range.
	ull mid = (seg_start + seg_end ) /2;
	updateRangeLazy(value, low, high, seg_start, mid, 2*index+1);
	updateRangeLazy(value, low, high, mid+1, seg_end, 2*index+2);
	segmentTree[index] = operation(segmentTree[2*index+1], segmentTree[2*index+2]);
}
ull SegmentTree::queryLazy(ull low, ull high, ull seg_start=0, ull seg_end=0, ull index=0)
{
	
	//Trick to detect first call and update high
	if(!seg_start && !seg_end && !index)
		 seg_end = input.size()-1;
	if ( (seg_start > seg_end) ||(seg_end < low) || (seg_start > high))
		return NO_OVERLAP;
	if(lazy[index])
	{
		segmentTree[index] += ((seg_end-seg_start+1)*lazy[index]); //Update and also push to child nodes
		if(seg_start!=seg_end)
		{
			lazy[2*index+1] += lazy[index];
			lazy[2*index+2] += lazy[index];
		}
		lazy[index] =0;
	}
	// complete overlap i.e. segment start & end should fit in the low & high range
	if(seg_start>= low && seg_end <=high)
		return segmentTree[index];
	// no overlap
	else
	{
		ull mid = (seg_start + seg_end ) /2;
		ull a = queryLazy(low, high, seg_start, mid, (2*index)+1); 
		ull b = queryLazy(low, high, mid+1, seg_end, (2*index)+2);
		return operation(a, b);
	}
}
void SegmentTree::debug()
{
	for (int j=0;j<size; j++)
			cout <<segmentTree[j] <<" ";
	cout <<endl;
	for (int j=0;j<size; j++)
			cout <<lazy[j] <<" ";
	cout <<endl;
}
int main()
{
	ios::sync_with_stdio(false);
#if 1
	ull t;
	cin >>t;
	for (ull i=0; i<t;i++)
	{
		ull N, C;
		cin >> N>> C;
		vector <ull> a(N, 0);
		class SegmentTree s(a);
		//s.debug();
		for (ull j =0; j<C; j++)
		{
			ull type, start, end , value;
			cin >> type; 
			if(type) // Query
			{
				cin >> start>>end;
				cout << s.queryLazy(start-1, end-1)<<"\n";
				//cout << s.query(start-1, end-1)<<endl;
			}
			else //update
			{
				cin >> start>>end>>value;
				s.updateRangeLazy(value, start-1, end-1);
				//s.updateRangeMultiple(value, start-1, end-1);
				//s.debug();
			}
		}
		
	}
#else
	//vector<ull> a =   {2, 4, 3, 1, 6, 7, 8, 9, 1, 7};
	vector<ull> a = {9, 8, 7, 1, 5, 4, 3, 2};
	ull i=0, j=9; // RMQ query between index
	
	class SegmentTree s(a);	
	cout << "[Segment_Tree] RMQ value is  "<< s.queryLazy(i, j) <<endl;
	s.updateRangeLazy(5, 1, 2);
	cout << "[Segment_Tree] RMQ value is  "<< s.queryLazy(2, 2) <<endl;
cout << "[Segment_Tree] RMQ value is  "<< s.queryLazy(1, 2) <<endl;
	cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	i=7; j=9;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	s.updateRange(2, 10);
	cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	i=0; j=3;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
	s.updateRangeMultiple(2, 5,9);
	i=5; j=9;cout << "[Segment_Tree] RMQ value is  "<< s.query(i, j) <<endl;
#endif

}
