#pragma once
#include <stdio.h>
#include <functional>
#include <string>
#include <vector>
#include "TimeTool.h"

struct AutoLogTimeStruct
{
	AutoLogTimeStruct(const char* name){
		this->name = name;
		startTime = getPerformanceTime();
	}
	~AutoLogTimeStruct(){
		printf("[AutoLogTime]%sºÄÊ±:%.2fs\n",name,getDiffTime(startTime,getPerformanceTime()));
	}
	PTime startTime;
	const char* name;
};

#define AutoLogTime(name) AutoLogTimeStruct autoLogTime(name)


class ProfileManager
{
public:
	ProfileManager(){}
	static ProfileManager* instance();
	int reg(const char* name);
	void addTime(int id,float time){
		totalTime[id] += time;
	}
	void getResult(const std::function<void(const char*,float)>&fun);
private:
	std::vector<std::string>names;
	std::vector<float>totalTime;
};

struct ProfileTime{
	ProfileTime(int id){
		startTime = getPerformanceTime();
		this->id = id;
	}
	~ProfileTime(){
		float diff = getDiffTime(startTime,getPerformanceTime());;
		ProfileManager::instance()->addTime(id,diff);
	}
private:
	PTime startTime;
	int id;
};


struct ProfileRunFirst{
	ProfileRunFirst(const char* name){
		id = ProfileManager::instance()->reg(name);
	} 
	int id; 
}; 

#define ProfileTotal(NAME) \
	static ProfileRunFirst s_ProfileRunFirst(NAME); \
	ProfileTime tempProfileTime(s_ProfileRunFirst.id);

