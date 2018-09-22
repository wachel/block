#include "debugtool.h"

ProfileManager* g_ProfileManager = NULL;

ProfileManager* ProfileManager::instance()
{
	if(!g_ProfileManager){
		g_ProfileManager = new ProfileManager;
	}
	return g_ProfileManager;
}

int ProfileManager::reg( const char* name )
{
	names.push_back(name);
	totalTime.push_back(0);
	return names.size() - 1;
}

void ProfileManager::getResult( const std::function<void(const char*,float)>&fun )
{
	for(int i = 0; i<names.size(); i++){
		fun(names[i].c_str(),totalTime[i]);
	}
}

