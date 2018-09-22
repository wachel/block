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
	bool bHit;//True:�������飬False:�뿪
	int x,y,z;//��ʾ���з���λ�ã�����ײʱ��λ�û���chunk����
	FaceNormalIndex faceIndex;//��ײ���߳������
	Real hitLength;//��������ʱ��ײ�ߵĳ��ȣ�����ײʱ���ᵽ��Chunk����
};

//���뱣֤startPos��Chunk�ڲ�
RayTraceResult rayTrace(const Vector3& startPos,const Vector3& dirInfo,const RayTraceChunk* grid,float length);


BlockNameSpaceEnd