#include "stdafx.h"

#include "core/core.h"
using namespace oo;

#include "RayTraceInterface.h"
#include "RayTrace/RayTrace.h"
#include "RayTrace/RayTraceChunkManager.h"

static std::map<int,RayTraceChunkManager*>managers;
static int g_maxID = 1;

DLLEXPORT int RayTrace_createManager(int chunkNumX, int chunkNumY, int chunkNumZ, int chunkSize, float blockLength)
{
	RayTraceChunkManager* manager = new RayTraceChunkManager();
	manager->create(chunkNumX,chunkNumY,chunkNumZ,chunkSize,blockLength);
	managers[g_maxID] = manager;
	g_maxID++;
	return g_maxID-1;
}

DLLEXPORT void RayTrace_deleteManager(int handle)
{
	delete managers[handle];
	managers[handle] = NULL;
}

DLLEXPORT void RayTrace_moveTo(int handle, int startChunkX, int startChunkY, int startChunkZ, RayTrace_GetChunkFun getChunk)
{
	RayTraceChunkManager* mgr = managers[handle];
	mgr->moveTo(startChunkX, startChunkY, startChunkZ, [getChunk, mgr](int x, int y, int z, std::vector<bool>& blocks){
		int* temp = new int[mgr->chunkSize*mgr->chunkSize*mgr->chunkSize];
		int length = 0;
		(*getChunk)(x, y, z, &temp, &length);
		blocks.resize(length);
		for (int i = 0; i < length; i++) {
			blocks[i] = (temp)[i] != 0;
		}
		delete[] temp;
	});
}

DLLEXPORT bool RayTrace_rayCast(int handle, float startX, float startY, float startZ, float dirX, float dirY, float dirZ, float length, float* outHitLength, int* outHitFaceIndex)
{
	RayTraceChunkManager* mgr = managers[handle];
	RayCastRestult rlt;
	mgr->rayCast(rlt,Vector3(startX,startY,startZ),Vector3(dirX,dirY,dirZ),length);
	*outHitFaceIndex = rlt.hitFaceIndex;
	*outHitLength = rlt.hitLength;
	return rlt.bHit;
}

DLLEXPORT void RayTrace_batchRayCast(int handle, int num, float* posArray, float* dirArray, float* lineLength, int* outBHit, float* outHitLength, int* outHitFaceIndex)
{
	RayTraceChunkManager* mgr = managers[handle];
	for (int i = 0; i < num; i++){
		RayCastRestult rlt;
		Vector3 startPos(posArray[i * 3], posArray[i * 3 + 1], posArray[i * 3 + 2]);
		Vector3 dir(dirArray[i * 3], dirArray[i * 3 + 1], dirArray[i * 3 + 2]);
		mgr->rayCast(rlt, startPos, dir, lineLength[i]);
		outHitFaceIndex[i] = rlt.hitFaceIndex;
		outHitLength[i] = rlt.hitLength;
		outBHit[i] = rlt.bHit?1:0;
	}
}

DLLEXPORT void ReadPixels(int x, int y, int width, int height, int format,int type, void* buffer)
{
	printf("ReadPixels");
	glReadPixels(x, y, width, height, format, type, buffer);
}

DLLEXPORT void TexSubImage3D(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, int format, int type, void* data)
{
	printf("TexSubImage3D");
	glTexSubImage3D(GL_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
}
