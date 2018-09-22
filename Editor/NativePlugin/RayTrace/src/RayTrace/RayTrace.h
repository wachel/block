#pragma once

#include "RayTraceChunk.h"

BlockNameSpaceBegin

enum FaceNormalIndex
{
	FNI_Unknown=-1,
	FNI_x0,
	FNI_y0,
	FNI_z0,
	FNI_x1,
	FNI_y1,
	FNI_z1,
	FNI_OutOfLength,
};

struct RayTraceResult
{
	RayTraceResult(bool bHit,int x,int y,int z,FaceNormalIndex faceIndex,Real hitLength){
		this->bHit = bHit;
		this->x = x;
		this->y = y;
		this->z = z;
		this->faceIndex = faceIndex;
		this->hitLength = hitLength;
	}
	bool bHit;//True:碰到方块，False:离开
	int x,y,z;//表示击中方块位置，无碰撞时此位置会在chunk外面
	FaceNormalIndex faceIndex;//碰撞法线朝向的面
	Real hitLength;//击中物体时碰撞线的长度，无碰撞时，会到达Chunk外面
};

//必须保证startPos在Chunk内部
RayTraceResult rayTrace(const Vector3& startPos,const Vector3& dirInfo,const RayTraceChunk* grid,float length);


BlockNameSpaceEnd