#include "VFileSystem.h"

OO_NAMESPACE_BEGIN
#pragma warning ( disable : 4996 )

bool VFileSystemSample::testFile( const String& file )
{
	FILE* p = fopen(file.c_str(),"rb");
	if(p){
		fclose(p);
		return true;
	}
	return false;
}

bool VFileSample::open()
{
	p = fopen(fileName.c_str(),mode.c_str());
	return p != 0;
}


OO_IMPL_CLASS(VFileData,OObject);
OO_IMPL_ABSTRACT(VFileBase,OObject);
OO_IMPL_CLASS(VFileSample,VFileBase);
OO_IMPL_ABSTRACT(VFileSystemBase,OObject);
OO_IMPL_CLASS(VFileSystemSample,VFileSystemBase);


OO_NAMESPACE_END