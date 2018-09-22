#pragma once

#include "RayTraceChunk.h"

BlockNameSpaceBegin

struct RayCastRestult{
	bool bHit;
	float hitLength;
	int hitFaceIndex;
};

class RayTraceChunkManager
{
public:
	RayTraceChunkManager(){}
	~RayTraceChunkManager(){}
	void create(int chunkNumX,int chunkNumY,int chunkNumZ,int chunkSize,float blockLength);
	void moveTo(int startChunkX,int startChunkY,int startChunkZ,const std::function<void(int,int,int,std::vector<bool>&)>&getChunk);
	void rayCast(RayCastRestult& outResult, const Vector3& startPos, const Vector3& dir, float length);
	int chunkSize;		//每个chunk的每个方向有多少个block
private:
	int sizeX,sizeY,sizeZ;	//每个方向有多少个chunk
	float blockLength;	//每个block的边长
	int curStartChunkX,curStartChunkY,curStartChunkZ;//起始区域是哪个chunk
	RayTraceChunk::Ptr bigChunk;//自身对应的粗糙chunk
	RayTraceChunk::Array chunks;//每个具体块
};

BlockNameSpaceEnd