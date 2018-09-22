#pragma once

BlockNameSpaceBegin

class RayTraceChunk:public OObject
{
	OO_DECL_CLASS(RayTraceChunk,OClass::flagConcrete,0){};
public:
	RayTraceChunk(){}
	virtual ~RayTraceChunk(){}
	void create(int dx,int dy,int dz,int blockSize,bool bHasChild){
		byteNumZ = (dz-1)/8+1;
		data.resize(dx*dy*byteNumZ);
		this->sizeX = dx; this->sizeY = dy; this->sizeZ = dz;
		this->bHasChild = bHasChild;
		if(bHasChild){
			children.resize(dx*dy*dz);
		}
		this->blockSize = blockSize;
	}
	inline int getSizeX()const{return sizeX;}
	inline int getSizeY()const{return sizeY;}
	inline int getSizeZ()const{return sizeZ;}
	inline int getBlockSize()const{return blockSize;}
	void clearAll(){
		memset(&data[0],0,sizeX*sizeY*byteNumZ);
	}
	inline void clearBlock(int x,int y,int z){
		data[y*sizeX*byteNumZ+x*byteNumZ+z/8] &= ~(1<<(z%8));
	}
	inline void setBlock(int x,int y,int z){
		data[y*sizeX*byteNumZ+x*byteNumZ+z/8] |= (1<<(z%8));
	}
	inline bool testBlock(int x,int y,int z)const{
		return (data[y*sizeX*byteNumZ+x*byteNumZ+z/8] & (1<<(z%8))) != 0;
	}
	inline void setChild(int x,int y,int z,RayTraceChunk* child){
		children[z*sizeY*sizeX + y*sizeX + x] = child;
	}
	const RayTraceChunk* getChild(int x,int y,int z)const{
		return children[z*sizeY*sizeX + y*sizeX + x];
	}
	bool isEmpty()const{return bEmpty;};
	bool isFull()const{return bFull;};
	bool hasChild()const{return bHasChild;}
	void updateEmptyAndFull();
private:
	std::vector<Byte>data;
	bool bHasChild;
	int blockSize;
	std::vector<RayTraceChunk::Ptr>children;
	int sizeX,sizeY,sizeZ;//尺寸
	int byteNumZ;//z轴由几个Byte组成
	bool bEmpty,bFull;
};

BlockNameSpaceEnd
