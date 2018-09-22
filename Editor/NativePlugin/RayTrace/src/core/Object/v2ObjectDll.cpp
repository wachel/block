// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#include "..\Common\v2ObjStd.h"
#include "v2objectdll.h"
//#include "v2LogStream.h"

OO_NAMESPACE_BEGIN

//static int numInit = 0;
//static Log* defaultLog = 0;
//
//std::vector<DynamicLib*> dllPlugins;
//PluginManager *pluginMgr = 0;
//
//VFileSystem *vfs = 0;
//Platform *platform = 0;
//
//typedef void (*FUN_dllPlugin)(void);
//typedef void (*FUN_dllPlugout)(void);
//
//static String s_defLogPath;
//
//// 这个函数在Client.dll::start调用都太晚了，目前改默认引擎日志用控制台命令"-eng_log_path PATH"
//OBJ_API void Object_setDefaultLogPath(const String &path)
//{
//	s_defLogPath = path;
//}
//
//OBJ_API const String& Object_getDefaultLogPath()
//{
//	return s_defLogPath;
//}
//
//OBJ_API Log* Object_getDefaultLog()
//{
//	return defaultLog;
//}
//
//OBJ_API void Object_loadDllPlugins(const String& path)
//{
//	ooGuard("Object_loadDllPlugins");
//
//	// reads in plugin file, gets dlls filename.
//	
//	//char dir[_MAX_PATH];	//  [1/21/2009 Archer]
//	//getcwd(dir, _MAX_PATH);
//
//	FILE *file = fopen(path.c_str(), "r");
//	if (!file)
//		ooThrow(EFileNotFound(path, "No plugin configure file found!"));
//
//	int len = Utility::GetFileLength(path);
//	String str;
//	str.resize(len);
//
//    fread(&(*str.begin()), len, 1, file);
//	fclose(file);
//
//	StringVector plugins = uString::split(str, "\r\n");
//
//	// calls all plugin's "dllPlugin" function.
//	for (StringVector::iterator it = plugins.begin(); it != plugins.end(); ++ it) {
//		String name = *it;
//		if (uString::left(name, 2) == "//" || name.empty() || name[0] == '\0') // 最后一行空行时会出现'\0'
//			continue;
//
//		logNotice(LM_Core, String("Loading plugin: \"") + *it + "\"");
//
//        // loads dll file.
//		DynamicLib *lib = DynamicLib::CreateDynamicLib(PathUtility::getModulePath() + "/" + (*it));
//		
//		// finds function address.
//		FUN_dllPlugin pFunc = (FUN_dllPlugin)lib->getFunctionAddress("dllPlugin");
//		if (!pFunc)
//			ooExcept(Exception::errGeneric, "DynamicLib::getFunctionAddress return 0 when load " + *it);
//
//		// call "dllPlugin"
//		(*pFunc)();
//
//		dllPlugins.push_back(lib);
//	}
//
//	//pluginMgr->fireStartPlugin("Platform");
//	pluginMgr->fireStartPlugin("RenderSystem");
//	pluginMgr->fireStartPlugin("Archive");
//	pluginMgr->fireStartPlugin("Font");
//	pluginMgr->fireStartPlugin("Input");
//	pluginMgr->fireStartPlugin("Media");
//}
//
//void unloadDllPlugins()
//{
//	ooGuard("unloadDllPlugins");
//
//	//pluginMgr->fireEndPlugin();
//
//	// notifies all dlls end plugin.
//
//	std::vector<DynamicLib*>::reverse_iterator it;
//	for (it = dllPlugins.rbegin(); it != dllPlugins.rend(); ++it) {
//		DynamicLib *lib = (DynamicLib*)(*it);
//
//		logNotice(LM_Core, "Unloading plugin file: \"" + lib->getName() + "\"");
//
//		// finds function address
//		FUN_dllPlugout pFunc = (FUN_dllPlugout)lib->getFunctionAddress("dllPlugout");
//		if (!pFunc)
//			ooExcept(Exception::errGeneric, "getFunctionAddress return 0 when unload.");
//
//		// call "dllPlugout"
//		(*pFunc)();
//	}
//
//	// removes all.
//	//dllPlugins.deleteAllPointers();
//	for (it = dllPlugins.rbegin(); it != dllPlugins.rend(); ++it) {
//		DynamicLib *lib = *it;
//		DynamicLib::ReleaseDynamicLib(lib);
//	}
//	dllPlugins.clear();
//}
//
//void Object_addPlugin(Plugin* plugin)
//{
//	ooGuard("Object_addPlugin");
//
//	pluginMgr->addPlugin(plugin);
//}
//
//void Object_init()
//{
//	ooGuard("Object_init");
//
//	if (numInit == 0) {
//		Core_init();
//		platform = new Platform;
//
//		if (s_defLogPath.empty())
//			s_defLogPath = Utility::GetModulePath() + "engine.log";
//
//		try {
//			defaultLog = new Log(s_defLogPath);
//		}
//		catch (...) {
//			defaultLog = 0; // may "engine.log" is readonly.
//		}
//
//		pluginMgr = new PluginManager;
//
//		vfs = new VFileSystem;
//		vfs->addPackageFactory(objNew(DirPackageFactory));
//	}
//
//	++numInit;
//}
//
//void Object_destroy()
//{
//	--numInit;
//
//	if (numInit == 0) {
//		pluginMgr->fireEndPlugin("Media");
//		pluginMgr->fireEndPlugin("Input");
//		//pluginMgr->fireEndPlugin("Font");
//		pluginMgr->fireEndPlugin("Archive");
//		pluginMgr->fireEndPlugin("RenderSystem");
//		
//		safeDelete(vfs);
//        		
//		unloadDllPlugins();
//		safeDelete(pluginMgr);
//
//		safeDelete(platform);
//
//		logNotice(LM_Core, "Origo Object is shutdown!");
//		safeDelete(defaultLog);
//
//		Core_destroy();
//	}
//}

OO_NAMESPACE_END
