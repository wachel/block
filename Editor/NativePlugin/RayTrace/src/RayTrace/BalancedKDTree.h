#pragma once


struct NodeData{
	NodeData(const Vector3& p,void* d){
		pos = p;
		pData = d;
	}
	Vector3 pos;
	void* pData;
};

class DistBuffer
{
public:
	DistBuffer(int num){
		dists.resize(num,Math::REAL_MAX * Math::REAL_MAX);
		maxIndex = 0;
	}
	inline Real getMaxDistSq(){
		return dists[maxIndex];
	}
	inline void update(Real dist){
		dists[maxIndex] = dist;
		//更新最大值
		Real maxDistSq = 0;
		maxIndex = 0;
		for(unsigned int i = 0; i<dists.size(); i++){// 找到最远的一个
			if(maxDistSq < dists[i]){
				maxDistSq = dists[i];
				maxIndex = i;
			}
		}
	}
	inline int getMaxIndex(){return maxIndex;}
private:
	std::vector<Real>dists;
	int maxIndex;
};

class KDNode
{
public:
	KDNode();
	~KDNode();
	void build(NodeData** buff,int num,int depth,const AABB& aabb);
	void nearest(const Vector3& pos,DistBuffer& dists,std::vector<NodeData*>&results);
	void release();
private:
	int axis;
	Real splitValue;
	AABB aabb;
	bool bLeaf;
	NodeData* data;
	KDNode* left;
	KDNode* right;
};

class KDTree
{
public:
	KDTree(){
		aabb.reset();
	}
	~KDTree(){}
	void addPoint(const Vector3& pos,void* pData){
		buff.push_back(NodeData(pos,pData));
		aabb.addPoint(pos);
	}
	void build(){
		ptrBuffer.resize(buff.size());
		for(unsigned int i = 0; i < buff.size(); i++){
			ptrBuffer[i] = &(buff[i]);
		}
		root.build(&ptrBuffer[0],ptrBuffer.size(),0,aabb);
	}
	void release(){
		root.release();
	}

	class Callback{
	public:
		Callback(){}
		virtual ~Callback(){}
		virtual bool isValid(void* data){return true;}
		virtual void result(void * data);
	};
	void getNearestN(const Vector3& pos,Callback* cb,std::vector<NodeData*>&results,Real& maxDistSq);
private:
	AABB aabb;
	KDNode root;
	std::vector<NodeData>buff;
	std::vector<NodeData*>ptrBuffer;
};

