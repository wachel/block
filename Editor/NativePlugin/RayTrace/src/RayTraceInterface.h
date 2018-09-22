
//typedef int(*callback) (int a, int b);
//const std::function<void(int, int, int, std::vector<bool>&)>&
#ifdef WIN32
#define STDCALL __stdcall
#define DLLEXPORT __declspec(dllexport)
#else
#define STDCALL 
#define DLLEXPORT 
#endif



typedef void(STDCALL *RayTrace_GetChunkFun)(int chx, int chy, int chz, int** outBlocks, int* length);
extern "C" DLLEXPORT int RayTrace_createManager(int chunkNumX, int chunkNumY, int chunkNumZ, int chunkSize, float blockLength);
extern "C" DLLEXPORT void RayTrace_deleteManager(int handle);
extern "C" DLLEXPORT void RayTrace_moveTo(int handle, int startChunkX, int startChunkY, int startChunkZ, RayTrace_GetChunkFun getChunk);
extern "C" DLLEXPORT bool RayTrace_rayCast(int handle, float startX, float startY, float startZ, float dirX, float dirY, float dirZ, float length,
			float* outHitLength,int* outHitFaceIndex);
extern "C" DLLEXPORT void RayTrace_batchRayCast(int handle, int num, float* posArray, float* dirArray, float* lineLength, int* outBHit, float* outHitLength, int* outHitFaceIndex);

extern "C" DLLEXPORT void ReadPixels(int x, int y, int width, int height,int format, int type, void* buffer);
extern "C" DLLEXPORT void TexSubImage3D(int level,int xoffset,int yoffset,int zoffset,int width,int height,int depth,int format,int type,void* data);
