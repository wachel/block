#include "..\Common\v2ObjStd.h"
#include "v2ObjBase.h"
#include "v2Object.h"
#include "..\Utility\v2String.h"

OO_NAMESPACE_BEGIN

class VFileData : public OObject
{
	OO_DECL_CLASS(VFileData, OClass::flagConcrete, 0) {}
public:
	VFileData() : buffer(0), size(0) {}
	virtual ~VFileData(){
		if(buffer){
			delete[] buffer;
			buffer = 0;
		}
	};

	void create(int sz){
		ooAssert(sz >= 0);
		if(buffer){
			delete[] buffer;
			buffer = 0;
		}

		// 07/1/26, add one byte of '\0' for text file to avoid error when (char *)buffer.
		// It is a dirty solution since the vfs is not support text file mode so far..
		buffer = new Byte[sz+1];
		buffer[sz] = '\0';

		size = sz;
	}

	Byte* buffer;
	int size;
};

class VFileBase:public OObject
{
	OO_DECL_ABSTRACT(VFileBase,OClass::flagAbstract,0){}
public:
	VFileBase(){
		//curPos = 0;
	}
	virtual ~VFileBase(){}
	virtual bool open(){return true;};
	virtual void close(){};
	virtual int getSize() = 0;
	virtual bool readByPos(char* buf,int pos,int size){
		setCurPos(pos);
		return read(buf,size);
	};
	virtual void writeByPos(const char* buf,int pos,int size){
		setCurPos(pos);
		write(buf,size);
	};
	virtual bool read(void* buf,int size) = 0;
	virtual void write(const void* buf,int size) = 0;
	virtual bool isEof() = 0;
	virtual int getCurPos() const = 0;
	virtual void setCurPos(int val) = 0;
	virtual VFileData::Ptr readAllData(){
		VFileData::Ptr fileData = objNew(VFileData);
		fileData->create(getSize());
		readByPos((char*)fileData->buffer,0,getSize());
		return fileData;
	}
	String fileName;
private:
	//int curPos;
};

class VFileSample:public VFileBase
{
	OO_DECL_CLASS(VFileSample, OClass::flagConcrete, 0) {}
public:
	VFileSample(){
		p = 0;
		mode = "rb";
	}
	virtual ~VFileSample(){
		close();
	}
	bool open();
	void close(){
		if(p){
			fclose(p);
			p = 0;
		}
	}
	virtual int getSize(){
		fseek(p,0,SEEK_END );
		return ftell(p); 
	}
	virtual bool read(void* buf,int size){
		fread(buf,size,1,p);
		return true;
	}
	virtual void write(const void* buf,int size){
		fwrite(buf,size,1,p);
	}
	virtual int getCurPos() const{
		return ftell(p);
	}
	virtual void setCurPos(int val){
		fseek(p,val,SEEK_SET);
	}
	virtual bool isEof(){
		return feof(p) != 0;
	}
	String mode;
private:
	FILE* p;
};


class VFileSystemBase:public OObject
{
public:
	OO_DECL_ABSTRACT(VFileSystemBase, OClass::flagAbstract, 0) {}
public:
	VFileSystemBase(){}
	virtual ~VFileSystemBase(){}
	virtual bool testFile(const String& file) = 0;
	virtual VFileBase::Ptr openFile(const String& file,const String& mode = "rb") = 0;
};

class VFileSystemSample:public VFileSystemBase
{
	OO_DECL_CLASS(VFileSystemSample, OClass::flagConcrete, 0) {}
public:
	VFileSystemSample(){}
	virtual ~VFileSystemSample(){}
	virtual bool testFile(const String& file);
	virtual VFileBase::Ptr openFile(const String& file,const String& mode = "rb"){
		if(mode.find('w') != -1){
			VFileSample* fp = objNew(VFileSample);
			fp->fileName = file;
			fp->mode = mode;
			fp->open();
			return (VFileSample*)fp;
		}
		else if(testFile(file)){
			VFileSample* fp = objNew(VFileSample);
			fp->fileName = file;
			fp->mode = mode;
			fp->open();
			return (VFileSample*)fp;
		}
		return NULL;
	}
};

OO_NAMESPACE_END