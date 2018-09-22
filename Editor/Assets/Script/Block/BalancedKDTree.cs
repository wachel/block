using UnityEngine;
using System.Collections;


//struct NodeData{
//    NodeData(Vector3 p){
//        pos = p;
//        pData = d;
//    }
//    Vector3 pos;
//};

//class DistBuffer
//{
//    public DistBuffer(int num){
//        dists = new float[num];
//        maxIndex = 0;
//    }
//    public float getMaxDistSq(){
//        return dists[maxIndex];
//    }
//    public void update(float dist){
//        dists[maxIndex] = dist;
//        //更新最大值
//        float maxDistSq = 0;
//        maxIndex = 0;
//        for(int i = 0; i<dists.Length; i++){// 找到最远的一个
//            if(maxDistSq < dists[i]){
//                maxDistSq = dists[i];
//                maxIndex = i;
//            }
//        }
//    }
//    public int getMaxIndex(){return maxIndex;}

//    float[] dists;
//    int maxIndex;
//};

//class KDNode
//{
//    public KDNode();
//    ~KDNode();
//    public void build(NodeData** buff,int num,int depth,const AABB& aabb);
//    public void nearest(const Vector3& pos,DistBuffer& dists,std::vector<NodeData*>&results);
//    public void release();

//    int axis;
//    Real splitValue;
//     aabb;
//    bool bLeaf;
//    NodeData* data;
//    KDNode* left;
//    KDNode* right;
//};

//class KDTree
//{
//public:
//    KDTree(){
//        aabb.reset();
//    }
//    ~KDTree(){}
//    void addPoint(const Vector3& pos,void* pData){
//        buff.push_back(NodeData(pos,pData));
//        aabb.addPoint(pos);
//    }
//    void build(){
//        ptrBuffer.resize(buff.size());
//        for(unsigned int i = 0; i < buff.size(); i++){
//            ptrBuffer[i] = &(buff[i]);
//        }
//        root.build(&ptrBuffer[0],ptrBuffer.size(),0,aabb);
//    }
//    void release(){
//        root.release();
//    }

//    class Callback{
//    public:
//        Callback(){}
//        virtual ~Callback(){}
//        virtual bool isValid(void* data){return true;}
//        virtual void result(void * data);
//    };
//    void getNearestN(const Vector3& pos,Callback* cb,std::vector<NodeData*>&results,Real& maxDistSq);
//private:
//    AABB aabb;
//    KDNode root;
//    std::vector<NodeData>buff;
//    std::vector<NodeData*>ptrBuffer;
//};

