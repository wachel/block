#include "../stdafx.h"
#include "../core/core.h"
using namespace oo;
#include "RandomPoints.h"
#include "BalancedKDTree.h"	


RandomPoints::RandomPoints(void)
{
}

RandomPoints::~RandomPoints(void)
{
}

const QuatArray& RandomPoints::getRandomPoints(int num)
{
	if(num >= randoms.size()){
		randoms.resize(num+1);
	}
	RandomTable& t = randoms[num];//��Ϊnum����̫����������Ϊ�˼ӿ��ٶȣ���num���±�ʹ��
	if(t.tables.size() == 0){
		makeTables(num,t.tables);	//��һ�η��ʴ�num������
	}
	int index = Math::rand(0,t.tables.size() - 1);//�ҵ�һ�����������
	return t.tables[index];
}

void RandomPoints::makeTables(int num,std::vector<QuatArray>& outTables)
{
	outTables.resize(10);
	for(unsigned int i = 0; i<outTables.size(); i++){
		buildArray(num,outTables[i]);
	}
}

//��Z�ᣬx������Ϊ0
inline Real getAngleFromDirection(const Vector3& dir)
{
	if(dir.x == 0){
		if(dir.y >= 0)
			return Math::PI*0.5;
		else
			return Math::PI*1.5;
	}
	Real angleYaw = atan(dir.y / dir.x);
	if(dir.x < 0){
		angleYaw += Math::PI;
	}
	return angleYaw;
}

void RandomPoints::buildArray(int num,QuatArray& outArray)
{
	//��������ԲƬ�������num����
	std::vector<Vector3>points;
	while(1){
		Real x = Math::rand(-100000,100000)/100000.0;
		Real y = Math::rand(-100000,100000)/100000.0;
		if(x * x + y * y < 1.0){
			points.push_back(Vector3(x,y,0));
			if(points.size() == num){
				break;
			}
		}
	}

	//����Щ�����ܶ�β����˶�ʹ����ɢ����,�����𽥽���
	for(int i = 0; i<20; i++){
		brownianMotion(i/20.0,points);
	}

	outArray.resize(num);
	//����ǰ�沽��õ��ľ��ȷ�ɢ�ĵ㣬�õ�Quaternion
	for(unsigned int i = 0; i<points.size(); i++){
		const Vector3& point = points[i];
		Real pitchAngle = Math::rAsin(point.length());
		Real yawAngle = getAngleFromDirection(point);
		outArray[i] = Point<Real>(pitchAngle,yawAngle);
	}
}

void RandomPoints::brownianMotion(Real temperature,std::vector<Vector3>& points)
{
	Real defaultDist = Math::rSqrt(Math::PI/points.size());
	KDTree kdtree;
	for(unsigned int i = 0; i<points.size(); i++){
		kdtree.addPoint(points[i],(void*)i);
	}
	kdtree.build();
	//ÿ���㾡��Զ�������㣬�൱��ÿ���㶼���ų����������
	std::vector<NodeData*>results;
	results.resize(3);//�ҵ����Լ������3����
	for(unsigned int i = 0; i<points.size(); i++){
		Vector3& point = points[i];
		Real outMaxDist = 0;
		kdtree.getNearestN(point,NULL,results,outMaxDist);
		Vector3 force = Vector3::ZERO;	//�ܵ����ų���
		for(unsigned int r = 0; r<results.size(); r++){
			NodeData* node = results[r];
			if(int(node->pData) != i){
				Vector3 dir = (point - node->pos);
				Real lengthSq = dir.lengthSq();
				if(lengthSq > 0.0000000000001){
					dir.normalize();
					force += dir * defaultDist * defaultDist * 0.15f * temperature/(lengthSq);
				}
				else{
					force.x += defaultDist*0.001*Math::rand(-1000,1000)/1000.0;
					force.y += defaultDist*0.001*Math::rand(-1000,1000)/1000.0;
				}
			}
		}
		force.z = 0;
		if(force.length() > defaultDist*0.2){
			force = force*defaultDist*0.2f /force.length();
		}
		point += force;
		point.x += defaultDist*0.001 *Math::rand(-1000,1000)/1000.0;
		point.y += defaultDist*0.001 *Math::rand(-1000,1000)/1000.0;
		if(point.lengthSq() > 0.9999){
			point = point/(point.length() + defaultDist*0.5 *Math::rand(100,1000)/1000.0);
		}
	}
}

