#pragma once

				//  pitch，yaw
typedef std::vector<Point<Real> > QuatArray;

struct RandomTable{
	std::vector<QuatArray>tables;//
};

class RandomPoints
{
public:
	RandomPoints(void);
	~RandomPoints(void);
	const QuatArray& getRandomPoints(int num);//获得一个随机的列表
	static void getNewDir(Real pitch,Real yaw,const Vector3& inDir,Vector3& outDir);
	void brownianMotion(Real temperature,std::vector<Vector3>& points);//布朗运动
private:
	void makeTables(int num,std::vector<QuatArray>& outTables);
	void buildArray(int num,QuatArray& outArray);
	std::vector<RandomTable>randoms;
};

inline void RandomPoints::getNewDir(Real pitch,Real yaw,const Vector3& inDir,Vector3& outDir)
{
	Vector3 axis;
	if(abs(inDir.z) > 0.9999){
		axis = inDir.cross(Vector3::UNIT_X);
	}
	else{
		axis = inDir.cross(Vector3::UNIT_Z);
	}
	Quaternion rotPitch;
	rotPitch.fromAxisAngle(axis,pitch);
	Quaternion rotYaw;
	rotYaw.fromAxisAngle(inDir,yaw);
	outDir = (rotYaw * rotPitch).transform(inDir);
}