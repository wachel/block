#include "../stdafx.h"
#include "../core/core.h"
using namespace oo;
#include "RayTraceChunkManager.h"
#include "RayTrace.h"


void RayTraceChunkManager::create(int chunkNumX,int chunkNumY,int chunkNumZ,int chunkSize,float blockLength)
{
	curStartChunkX = curStartChunkY = curStartChunkZ = -65536;

	sizeX = chunkNumX;	sizeY = chunkNumY;	sizeZ = chunkNumZ;
	this->chunkSize = chunkSize;
	this->blockLength = blockLength;
	bigChunk = objNew(RayTraceChunk);
	bigChunk->create(sizeX,sizeY,sizeZ,chunkSize,true);
	bigChunk->clearAll();
	chunks.resize(sizeX * sizeY * sizeZ);
	for(int k = 0; k<sizeZ; k++){
		for(int j = 0; j<sizeY; j++){
			for(int i = 0; i<sizeX; i++){
				RayTraceChunk::Ptr temp = objNew(RayTraceChunk);
				temp->create(chunkSize,chunkSize,chunkSize,1,false);
				temp->clearAll();
				chunks[k*sizeY*sizeX + j*sizeX + i] = temp;
				bigChunk->setChild(i,j,k,temp);
			}
		}
	}
}

void updateChunk(const std::vector<bool>& data,int chunkSize,RayTraceChunk* chunk)
{
	chunk->clearAll();
	for(int k = 0; k<chunkSize;k++){
		for(int j = 0;j<chunkSize;j++){
			for(int i = 0;i<chunkSize;i++){
				if(data[k*chunkSize*chunkSize + j*chunkSize + i]){
					chunk->setBlock(i,j,k);
				}
			}
		}
	}
	chunk->updateEmptyAndFull();
}

void RayTraceChunkManager::moveTo( int newStartChunkX,int newStartChunkY,int newStartChunkZ,const std::function<void(int,int,int,std::vector<bool>&)>&getChunk )
{
	RayTraceChunk::Array oldChunks;
	oldChunks.resize(chunks.size());
	for(unsigned int i = 0; i<chunks.size();i++){
		oldChunks[i] = chunks[i];
	}
	int dx = newStartChunkX - curStartChunkX;
	int dy = newStartChunkY - curStartChunkY;
	int dz = newStartChunkZ - curStartChunkZ;
	std::vector<bool>temp;
	temp.resize(chunkSize * chunkSize * chunkSize);
	bigChunk->clearAll();
	for(int k = 0; k<sizeZ; k++){
		for(int j = 0;j<sizeY; j++){
			for(int i = 0;i<sizeX; i++){
				if (i == 0 && j == 0 && k == 1) {
					int a = 0;
				}
				int fromX = i + dx;
				int fromY = j + dy;
				int fromZ = k + dz;
				chunks[k*sizeY*sizeX + j*sizeX + i] = oldChunks[(fromZ%sizeZ)*sizeY*sizeX + (fromY%sizeY)*sizeX + (fromX%sizeX)];
				if(fromX < 0 || fromX >= sizeX || fromY < 0 || fromY >= sizeY || fromZ < 0 || fromZ >= sizeZ){
					//此格子不在原范围，需要更新
					getChunk(newStartChunkX+i,newStartChunkY+j,newStartChunkZ+k,temp);
					updateChunk(temp,chunkSize,chunks[k*sizeY*sizeX + j*sizeX + i]);
				}
				if(!chunks[k*sizeY*sizeX + j*sizeX + i]->isEmpty()){
					bigChunk->setBlock(i,j,k);
				}
			}
		}
	}
	curStartChunkX = newStartChunkX;
	curStartChunkY = newStartChunkY;
	curStartChunkZ = newStartChunkZ;
}

void RayTraceChunkManager::rayCast( RayCastRestult& outResult,const Vector3& startPos,const Vector3& dir,float length )
{
	float outLength = 0;
	//求局部坐标，左下角为(0,0,0),单位长度为chunkSize
	Vector3 localStartPos = (startPos/(blockLength*chunkSize)) - Vector3(curStartChunkX,curStartChunkY,curStartChunkZ);
	RayTraceResult rltBig = rayTrace(localStartPos,dir,bigChunk,length);//进行粗糙范围射线追踪
	outResult.bHit = rltBig.bHit;
	if(rltBig.hitLength > length){
		outResult.bHit = false;
	}
	outResult.hitFaceIndex = rltBig.faceIndex;
	outResult.hitLength = rltBig.hitLength;
	//while(1){
	//	RayTraceResult rltBig = rayTrace(localStartPos,dir,bigChunk);//进行粗糙范围射线追踪
	//	outLength += rltBig.hitLength * blockLength * chunkSize;
	//	if(rltBig.bHit && outLength < length){
	//		RayTraceChunk* chunk = chunks[rltBig.z*sizeY*sizeX + rltBig.y*sizeX + rltBig.x];
	//		Vector3 hitPos = localStartPos + dir * rltBig.hitLength;
	//		Vector3 localStartPosInChunk = (hitPos - Vector3(rltBig.x,rltBig.y,rltBig.z)) * chunkSize;
	//		RayTraceResult rltSmall = rayTrace(localStartPosInChunk,dir,chunk);
	//		outLength += rltSmall.hitLength * blockLength;
	//		if(outLength < length){
	//			if(rltSmall.bHit){
	//				outResult.bHit = true;
	//				outResult.hitFaceIndex = rltSmall.faceIndex == FNI_Inner?rltBig.faceIndex:rltSmall.faceIndex;
	//				outResult.hitLength = outLength;
	//				return;
	//			}
	//			else{//如果穿出这个chunk，则需要从穿出面继续检测
	//				localStartPos += dir * rltSmall.hitLength/chunkSize;
	//				//localStartPos = (startPos + dir*outLength)/(blockLength*chunkSize) - Vector3(curStartChunkX,curStartChunkY,curStartChunkZ);
	//				continue;
	//			}
	//		}
	//		else{
	//			outResult.bHit = false;
	//			return;
	//		}
	//	}
	//	else{
	//		outResult.bHit = false;
	//		return;
	//	}
	//}
}

