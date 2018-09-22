#include "../stdafx.h"
#include "../core/core.h"
using namespace oo;
#include "RayTrace.h"



struct DirInfo
{
	DirInfo(const Vector3& dir){
		float reciprocalX = dir.x==0?Math::REAL_MAX:1/dir.x;
		float reciprocalY = dir.y==0?Math::REAL_MAX:1/dir.y;
		float reciprocalZ = dir.z==0?Math::REAL_MAX:1/dir.z;

		stepX = dir.x>=0?1:-1;
		stepY = dir.y>=0?1:-1;
		stepZ = dir.z>=0?1:-1;

		tDeltaX = Math::rAbs(reciprocalX);
		tDeltaY = Math::rAbs(reciprocalY);
		tDeltaZ = Math::rAbs(reciprocalZ);

		this->dir = dir;
	}
	int stepX;	//1或-1，表示射线的方向
	int stepY; 
	int stepZ; 
	Real tDeltaX;//每跨过1个X需要直线前进多少距离
	Real tDeltaY;
	Real tDeltaZ;
	Vector3 dir;
};

RayTraceResult rayTrace2(const Vector3& startPos,const DirInfo& dirInfo,const RayTraceChunk* grid,FaceNormalIndex faceIn,float maxLength);

enum TestBlockResult{
	TBR_Unknow,
	TBR_Empty,
	TBR_Full,
};

//TestBlockResult testBlock(const RayTraceChunk* grid,int x, int y,int z){
//	if(!grid->hasChild()){
//		if(grid->testBlock(x,y,z)){
//			return TBR_Full;
//		}
//		else{
//			return TBR_Empty;
//		}
//	}
//	else{
//		const RayTraceChunk* child = grid->getChild(x,y,z);
//		if(child->isFull()){
//			return TBR_Full; 
//		}
//		else if(child->isEmpty()){
//			return TBR_Empty;
//		}
//		else{
//			return TBR_Unknow;
//		}
//	}
//}

bool testBlockRecursive(RayTraceResult& rlt,const RayTraceChunk* grid,int x, int y,int z,const Vector3& startPos,const DirInfo& dirInfo,float curLength,FaceNormalIndex faceIn,float maxLength)
{
	if(!grid->hasChild()){
		if(grid->testBlock(x,y,z)){
			rlt = RayTraceResult(true,x,y,z,faceIn,curLength);
			return true;
		}
		else{
			return false;
		}
	}
	else{
		float blockSize = grid->getBlockSize();
		const RayTraceChunk* child = grid->getChild(x,y,z);
		if(child->isFull()){
			rlt = RayTraceResult(true,x*blockSize,y*blockSize,z*blockSize,faceIn,curLength*blockSize);
			return true;
		}
		else if(child->isEmpty()){
			return false;
		}
		else{
			Vector3 localStartPos = startPos + dirInfo.dir * curLength - Vector3(x,y,z);
			localStartPos *= (blockSize/child->getBlockSize());
			RayTraceResult rlt2 = rayTrace2(localStartPos,dirInfo,child,faceIn,maxLength);
			if(rlt2.bHit){
				rlt = RayTraceResult(true,x*blockSize+rlt2.x,y*blockSize+rlt2.y,z*blockSize+rlt2.z,rlt2.faceIndex,curLength*blockSize+rlt2.hitLength);
				return true;
			}
			else{
				return false;
			}
		}
	}
}

RayTraceResult rayTrace2(const Vector3& startPos,const DirInfo& dirInfo,const RayTraceChunk* grid,FaceNormalIndex faceIn,float maxLength)
{
	RayTraceResult rlt(false,0,0,0,FNI_Unknown,0);
	int X = int(startPos.x-0.000001);
	int Y = int(startPos.y-0.000001);
	int Z = int(startPos.z-0.000001);
	Real tMaxX = (dirInfo.stepX>0?(1 - (startPos.x-X)):(startPos.x - X)) * dirInfo.tDeltaX;//沿射线走多远才能跳到下一个X格
	Real tMaxY = (dirInfo.stepY>0?(1 - (startPos.y-Y)):(startPos.y - Y)) * dirInfo.tDeltaY;
	Real tMaxZ = (dirInfo.stepZ>0?(1 - (startPos.z-Z)):(startPos.z - Z)) * dirInfo.tDeltaZ;

	//考虑从上一级的格子进入这一级的情况，因为这时候起点刚好在边缘处，因此需要特殊处理

	if(faceIn == FNI_x0 || faceIn == FNI_x1){
		X = (faceIn == FNI_x0?0:(grid->getSizeX()-1));
		bool bHit = testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,0,faceIn,maxLength);
		if(bHit){
			return rlt;
		}
		X += dirInfo.stepX;
		tMaxX = dirInfo.tDeltaX;
	}
	else if(faceIn == FNI_y0 || faceIn == FNI_y1){
		Y = (faceIn == FNI_y0?0:(grid->getSizeY()-1));
		bool bHit = testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,0,faceIn,maxLength);
		if(bHit){
			return rlt;
		}
		Y += dirInfo.stepY;
		tMaxY = dirInfo.tDeltaY;
	}
	else if(faceIn == FNI_z0 || faceIn == FNI_z1){
		Z = (faceIn == FNI_z0?0:(grid->getSizeZ()-1));
		bool bHit = testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,0,faceIn,maxLength);
		if(bHit){
			return rlt;
		}
		Z += dirInfo.stepZ;
		tMaxZ = dirInfo.tDeltaZ;
	}
	else{
		if(grid->testBlock(X,Y,Z)){//如果从最外面进入，且当前格子不为空，则做深度检测
			bool bHit = testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,0,faceIn,maxLength);
			if(bHit){
				return rlt;
			}			
		}
	}

	while(1) {
		if(tMaxX < tMaxY) {
			if(tMaxX < tMaxZ) {
				X += dirInfo.stepX;
				if(X < 0 || X >= grid->getSizeX()){
					return RayTraceResult(false,X,Y,Z,dirInfo.stepX>0?FNI_x0:FNI_x1,tMaxX);
				}
				else if(testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,tMaxX,dirInfo.stepX>0?FNI_x0:FNI_x1,maxLength-tMaxX*grid->getBlockSize())){
					return rlt;
				}
				else if(tMaxX>maxLength){
					return RayTraceResult(false,X,Y,Z,FNI_OutOfLength,tMaxX);
				}
				tMaxX += dirInfo.tDeltaX;	
			} 
			else {
				Z += dirInfo.stepZ;
				if(Z < 0 || Z >= grid->getSizeZ()){
					return RayTraceResult(false,X,Y,Z,dirInfo.stepZ>0?FNI_z0:FNI_z1,tMaxZ);
				}
				else if(testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,tMaxZ,dirInfo.stepZ>0?FNI_z0:FNI_z1,maxLength-tMaxZ*grid->getBlockSize())){
					return rlt;
				}
				else if(tMaxZ>maxLength){
					return RayTraceResult(false,X,Y,Z,FNI_OutOfLength,tMaxZ);
				}
				tMaxZ += dirInfo.tDeltaZ;	
			}
		} 
		else {
			if(tMaxY < tMaxZ) {
				Y += dirInfo.stepY;
				if(Y < 0 || Y >= grid->getSizeY()){
					return RayTraceResult(false,X,Y,Z,dirInfo.stepY>0?FNI_y0:FNI_y1,tMaxY);
				}	
				else if(testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,tMaxY,dirInfo.stepY>0?FNI_y0:FNI_y1,maxLength-tMaxY*grid->getBlockSize())){
					return rlt;
				}
				else if(tMaxY>maxLength){
					return RayTraceResult(false,X,Y,Z,FNI_OutOfLength,tMaxY);
				}
				tMaxY += dirInfo.tDeltaY;
			} 
			else {
				Z += dirInfo.stepZ;
				if(Z < 0 || Z >= grid->getSizeZ()){
					return RayTraceResult(false,X,Y,Z,dirInfo.stepZ>0?FNI_z0:FNI_z1,tMaxZ);
				}	
				else if(testBlockRecursive(rlt,grid,X,Y,Z,startPos,dirInfo,tMaxZ,dirInfo.stepZ>0?FNI_z0:FNI_z1,maxLength-tMaxZ*grid->getBlockSize())){
					return rlt;
				}
				else if(tMaxZ>maxLength){
					return RayTraceResult(false,X,Y,Z,FNI_OutOfLength,tMaxZ);
				}
				tMaxZ += dirInfo.tDeltaZ;	
			}
		}
	}
}


RayTraceResult rayTrace(const Vector3& startPos,const Vector3& dir,const RayTraceChunk* grid,float length)
{
	DirInfo dirInfo(dir);
	Vector3 startPosInner = startPos;
	Math::clampMinMax(startPosInner.x,0,grid->getSizeX()-0.001);
	Math::clampMinMax(startPosInner.y,0,grid->getSizeY()-0.001);
	Math::clampMinMax(startPosInner.z,0,grid->getSizeZ()-0.001);
	return rayTrace2(startPosInner,dirInfo,grid,FNI_Unknown,length);
}

