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
	int chunkSize;		//ÿ��chunk��ÿ�������ж��ٸ�block
private:
	int sizeX,sizeY,sizeZ;	//ÿ�������ж��ٸ�chunk
	float blockLength;	//ÿ��block�ı߳�
	int curStartChunkX,curStartChunkY,curStartChunkZ;//��ʼ�������ĸ�chunk
	RayTraceChunk::Ptr bigChunk;//�����Ӧ�Ĵֲ�chunk
	RayTraceChunk::Array chunks;//ÿ�������
};

BlockNameSpaceEnd