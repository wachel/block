#define STDCALL __stdcall
#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT int fnGLWrapper(void);
extern "C" DLLEXPORT void ReadPixels(int x, int y, int width, int height, int format, int type, void* buffer);
extern "C" DLLEXPORT void TexSubImage3D(int garget, int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, int format, int type, void* data);
extern "C" DLLEXPORT int GetError();
