#include "../stdafx.h"
#include "../core/core.h"
using namespace oo;

#include "RayTraceChunk.h"
BlockNameSpaceBegin

void RayTraceChunk::updateEmptyAndFull()
{
	bFull = true;
	for(unsigned int i = 0; i<data.size(); i++){
		if(data[i] != 0xff){
			bFull = false;
			break;
		}
	}
	bEmpty = true;
	for(unsigned int i = 0; i<data.size(); i++){
		if(data[i] != 0){
			bEmpty = false;
			break;
		}
	}
}


OO_IMPL_CLASS(RayTraceChunk,OObject);


BlockNameSpaceEnd