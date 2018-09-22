// GLWrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GLWrapper.h"

DLLEXPORT void ReadPixels(int x, int y, int width, int height, int format, int type, void* buffer)
{
	glReadPixels(x, y, width, height, format, type, buffer); 
}

DLLEXPORT void TexSubImage3D(int target,int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, int format, int type, void* data)
{
	unsigned char *buffer = new unsigned char[width * height * depth];
	//glTexSubImage3D(target,level, xoffset, yoffset, zoffset, width, height, depth, format, type, buffer);
	int a = 0;
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_POINT);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_ALPHA, GL_UNSIGNED_BYTE, &a);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0,0, 0, 1,1, 1, GL_ALPHA, GL_UNSIGNED_BYTE, &a);
	int b = 0;
}   

DLLEXPORT int GetError()
{
	return glGetError();
}
