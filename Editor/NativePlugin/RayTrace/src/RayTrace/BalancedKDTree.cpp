#include "../stdafx.h"
#include "../core/core.h"
using namespace oo;
#include "BalancedKDTree.h"

using namespace oo;

KDNode::KDNode()
{
	left = right = NULL;
}

KDNode::~KDNode()
{

}

bool cmpx(const NodeData* a,const NodeData* b) {return a->pos.x < b->pos.x;}
bool cmpy(const NodeData* a,const NodeData* b) {return a->pos.y < b->pos.y;}
bool cmpz(const NodeData* a,const NodeData* b) {return a->pos.z < b->pos.z;}

inline void setAxisValue(Vector3& pos,int axis,Real v){
	(&(pos.x))[axis] = v;
}
inline Real getAxisValue(const Vector3& pos,int axis){
	return (&(pos.x))[axis];
}

void getAABB(const AABB& aabb,int axis,Real split,AABB& min,AABB& max)
{
	min = max = aabb;
	setAxisValue(min.vMax,axis,split);
	setAxisValue(max.vMin,axis,split);
}

void KDNode::build(NodeData** buff,int num,int depth,const AABB& aabb)
{
	if(num > 1){
		bLeaf = false;
		axis = depth % 3;
		this->aabb = aabb;
		std::sort(buff,buff + num,(axis==0)?cmpx:(axis==1)?cmpy:cmpz);
		int midIndex = num / 2;
		NodeData* midData = buff[midIndex];
		AABB leftAabb,rightAabb;
		getAABB(aabb,axis,getAxisValue(midData->pos,axis),leftAabb,rightAabb);
		left = new KDNode;
		left->build(buff,midIndex,depth + 1,leftAabb);
		if(num > 2){
			right = new KDNode;
			right->build(buff + midIndex + 1,num - midIndex - 1,depth + 1,rightAabb);
		}
		else{
			right = NULL;
		}
		data = midData;
		splitValue = getAxisValue(data->pos,axis);
	}
	else{
		bLeaf = true;
		this->aabb = aabb;
		data = buff[0];
	}
}

void KDNode::nearest(const Vector3& pos,DistBuffer& dists,std::vector<NodeData*>&results)
{
	Real distSq = (pos - data->pos).lengthSq();

	if(distSq < dists.getMaxDistSq()){
		results[dists.getMaxIndex()] = data;
		dists.update(distSq);
	}

	if(left){
		Real distToNear = getAxisValue(pos,axis) - splitValue;
		if((getAxisValue(pos,axis) < splitValue) || distToNear * distToNear < dists.getMaxDistSq()){
			left->nearest(pos,dists,results);			
		}
	}
	if(right){
		Real distToNear = getAxisValue(pos,axis) - splitValue;
		if((getAxisValue(pos,axis) > splitValue) || distToNear * distToNear < dists.getMaxDistSq()){
			right->nearest(pos,dists,results);
		}
	}
}

void KDNode::release()
{
	if(left){
		left->release();
		delete left;
		left = 0;
	}
	if(right){
		right->release();
		delete right;
		right = 0;
	}
}

void KDTree::getNearestN(const Vector3& pos,Callback* cb,std::vector<NodeData*>&results,Real& maxDistSq)
{
	DistBuffer distBuff(results.size());
	root.nearest(pos,distBuff,results);
	maxDistSq = distBuff.getMaxDistSq();
}