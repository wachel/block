// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
#ifndef OO_UTILITY_H_
#define OO_UTILITY_H_

#include "v2String.h"

OO_NAMESPACE_BEGIN

// @deprecated !!! - use safeDelete
#define SafeDelete(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SafeDeleteArray(p)	{ if(p) { delete[] (p);   (p)=NULL; } }

#define safeDelete(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define safeDeleteArray(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define safeRelease(p)      { if(p) { (p)->release(); (p)=NULL; } }

// Asserts expression is true at compile-time.
#define COMPILER_ASSERT(x) typedef int COMPILER_ASSERT_[!!(x)]

// Declare an unimplemented method.
#define OO_UNIMPLEMENTED_METHOD ooAssertDesc(false, "Not yet finish!");

// Declare an deprecated method.
#define OO_DEPRECATED_METHOD { static int __numEntry = 0; if (!__numEntry) { logWarning("Use a deprecated method. " __FUNCDNAME__); __numEntry++; }}

// Declare an temporary method(or unoptimized method)
#define OO_TEMP_METHOD { static int __numEntry = 0; if (!__numEntry) { logWarning("Use a temporary method. " __FUNCDNAME__); __numEntry++; }}

// break
//
//
//class Timer;
//
///** 基础的工具类。
//*/
//class OO_API Utility
//{
//public:
//
//	/** 返回系统时间，按秒。
//		
//		Win32平台下返回Windows启动后的流逝时间，否则返回调用进程的时间。
//	*/
//	static Real getTime();
//
//	/** 返回引擎缺省的计时器。 */
//	static Timer& getTimer();
//
//	// 废弃，命名不规范。用 Utility::getTime。
//	static Real GetTime() {
//		return getTime();
//	}
//
//// 以下路径相关的函数都废弃了，请用 PathUtility 。
//
//	// 废弃，用PathUtility::getFileLength
//	static Uint GetFileLength(const String& pathname);
//
//	// eg. GetAbsolutePath("../Resources", "d:/Origo/debug/") returns "d:\Origo\Resources".
//	static String GetAbsolutePath(const String& path, const String &rootDir = ".");
//
//	/* Translates absolute path to relative path
//		@remarks
//			eg. GetRelativePath("d:\Origo\Resources", "d:/Origo") returns "Resources/".
//		@note
//			can not support that likes: GetRelativePath("d:\Origo\Resources", "d:/Origo/exe") 
//	*/
//	static String GetRelativePath(const String& path, const String &rootDir);
//
//	/* Translates relative pathname to absolute pathname
//		@remarks
//			eg. GetAbsolutePathName("../Resources/map.bin") returns "d:\Origo\Resources\map.bin".
//				GetAbsolutePathName("../..") returns "d:\".
//	*/
//	static String GetAbsolutePathName(const String& pathname);
//	
//	// eg. GetPathFromPathName("d:/Origo/Resources/map.bin") returns "d:\Origo\Resources\".
//	static String GetPathFromPathName(const String& pathname);
//
//	// eg. GetNameFromPathName("d:/Origo/Resources/map.bin") returns "map.bin".
//	static String GetNameFromPathName(const String& pathname);
//
//	// eg. GetFileNameExtension("d:/Origo/Resources/map.bin") returns "BIN".
//	static String GetFileNameExtension(const String& filename);
//
//	// eg. GetFileNameWithoutExtension("d:/Origo/Resources/map.bin") returns "map".
//	static String GetFileNameWithoutExtension(const String& filename);
//	static String RemoveExtension(const String& filename);
//
//	// eg. "d:/Origo/bin".
//	static String GetModulePath();
//
//	/*
//	  eg. ValidaetPath("Origo/Resources") returns "Origo/Resources/".
//		 eg. ValidaetPath("Origo\Resources\") returns "Origo/Resources/".
//	*/
//	static String CheckPath(const String &path);
//
//};
//
///** 处理文件路径用的工具类。
//
//	这里需要的路径都是“/”这样分割的路径，用 toEnginePath 可以转换"\"到"/"。
//*/
//class OO_API PathUtility
//{
//public:
//	// 取得执行文件路径。eg. "d:/Origo/bin".
//	static String getModulePath();
//
//	// eg. "D:\AaAa\BbBb" - > "d:/aaaa/bbbb"
//	// 引擎内部使用，一般不要转换大小写。
//	static String checkPath(const String &path);
//
//	// eg. "D:\AaAa\BbBb" - > "D:/AaAa/BbBb"
//	// 转换到引擎内部用的路径（以"/"分割"）
//	static String toEnginePath(const String &path) {
//		return uString::replace(path, '\\', '/');
//	}
//
//	// eg. "d:/aaa/bbb.bmp" -> "d:/aaa".
//	static String removeName(const String& path);
//
//	// eg. "d:/aaa/bbb/" -> "d:/aaa/bbb".
//	static String removeLastSlash(const String& path);
//
//	// return removeLastSlash(head) + "/" + tail.
//	static String join(const String& head, const String& tail) {
//		return head.empty() ? tail : removeLastSlash(head) + "/" + tail;
//	}	
//
//	// eg. "d:/aaa/bbb.bMp" -> "bMp".
//	static String getExtension(const String& path);
//
//	// eg. "d:/aaa/bbb.bmp" -> "d:/aaa/bbb".
//	static String removeExtension(const String& path);
//
//	// eg. "d:/aaa/bbb.bmp" -> "bbb.bmp".
//	static String getName(const String& path);
//
//	// eg. "d:/aaa/bbb.bmp" -> "bbb".
//	static String getNameNoExt(const String& path);
//
//	static bool getFilesInDir(StringVector &name, const String& dir);
//	static bool getDirsInDir(StringVector &name, const String& dir);
//
//	// eg. 	makeDir("e:/temp/b/c/d", true);
//	static bool makeDir(const String& dir, bool recursive = false);
//	bool deleteDir(const String &dir);
//
//	// eg. 	isDir("e:/temp/b/c/d");
//	static bool isDir(const String& dir);
//	
//	/** 取得文件长度。
//		
//		@param pathname 必须是一个绝对路径。
//	*/
//	static Uint getFileLength(const String& pathname);
//
//	/** 取得有大小写的文件名字。
//		@param path 文件路径。
//		@return 有大小写的文件名字。
//
//		该函数通过查找磁盘文件，查找一个文件在磁盘上带大小写信息的名字。
//		path所指的文件必须存，否则返回""。
//		eg: "d:/myfile.bmp" -> "MyFile.bmp"
//	*/
//	static String getFileNameWithCase(const String &path);
//};
//
//class OO_API FPSRecorder
//{
//public:
//	FPSRecorder();
//	~FPSRecorder() {}
//
//	void update(Real timePos);
//	Real getFPS() const { return fps; }
//
//protected:
//
//	Real fps;
//
//	Real timeFrameOld;
//	Real timeFpsOld;
//	Uint tickCount;
//};
//
///** 计时器。
//*/
//class OO_API Timer
//{
//public:
//	Timer() : parentTimer(0) { reset(); }
//	Timer(Timer *parent) : parentTimer(parent) { reset(); }
//
//	/** 取得当前时间。单位：秒。
//	*/
//	Real getCurTime() const {
//		return curTime;
//	}
//	/** 取得当前帧（通过 @ref getCurTime 返回）再前一帧的时间。
//		
//		@return 时间。
//	*/
//	Real getLastTime() const {
//		return lastTime;
//	}
//	/** 取得两桢间的时间差。单位：秒。
//		@return deltaTime 时间差，只保证 >= 0，有可能返回0。
//	*/
//	Real getDeltaTime() const {
//		return deltaTime;
//	}
//
//	/** 更新计时器。
//		应用程序必须调用 update 计时器才会更新当前时间数据，这个函数一
//		般是主循环中每次循环调用一次。
//	*/
//	void update(){
//		update(getParentTime());
//	}
//
//	/** 重设计时器。
//		
//		@param cur 当前时间，决定了 @ref getCurTime 的返回值。
//		@param delta 时间差，决定了 @ref getDeltaTime 的返回值。
//		@param frame 累计帧数，决定了 @ref getFrameCount 的返回值。
//	*/
//	void reset(Real cur = 0, Real delta = 0, Uint frame = 0) {
//		Real parentTime = getParentTime();
//		baseTime = parentTime - cur;
//		pauseCount = 0;
//		totalPausedTime = 0.f;
//		totalDeltaTime = 0.f;
//
//		curTime = 0.f;
//		frameCount = 0;
//
//		update(parentTime);
//
//		deltaTime = delta;
//		frameCount = frame;
//	}
//	/** 暂停。
//
//		@return 暂停计数。
//
//		计时器里有一 @b 暂停计数 用来判断是否“真正暂停”，每次调用 @ref pause
//		和 @ref resume 暂停计数会加1和减1，当暂停计数为0时计时器才“真正暂停”。
//	*/
//	int pause() {
//		pauseCount ++;
//		startPauseTime = getParentTime();
//		return pauseCount;
//	}
//
//	/** 继续。
//
//		@return 暂停计数。
//		@see pause
//	*/
//	int resume() {
//		pauseCount --;
//		ooBreakAssert(pauseCount >= 0);
//		if (pauseCount == 0) {
//			// accumulate the paused time.
//			totalPausedTime += getParentTime() - startPauseTime;
//
//			startPauseTime = 0.f;
//		}
//		return pauseCount;
//	}
//	/** 是否暂停。
//		@return 是否暂停。
//	*/
//	bool isPaused() const {
//		return pauseCount > 0;
//	}
//	/** 取得累计帧数。
//		
//		@return 累计帧数，[0, 0xefffffff]。
//
//		每次调用 update 时累计帧数就会加1，超出最大值后会再从0开始。
//	*/
//	Uint getFrameCount() const { return frameCount; }
//
//	
//	/** 取得下一次 update 后的累计帧数。
//		
//		@return 累计帧数。
//		@see getFrameCount
//	*/
//	Uint getNextFrameCount() const { 
//		Uint next = frameCount + 1; next &= 0xefffffff; return next; // avoid overflow, clamp in[0, 0xefffffff]
//	}
//
//	/** 取得FPS。
//		
//		@return 每秒平均帧数。
//	*/
//	Real getFPS() const {
//		return fpsRecorder.getFPS();
//	}
//
//	/** 取得父类计时器。
//		
//		@return 父类计时器， NULL 表示没有父类计时器，用系统时间。
//	*/
//	Timer* getParentTimer() {
//		return parentTimer;
//	}
//
//	/** 设置父类计时器。
//
//		@param timer 父类计时器， NULL 表示没有父类计时器，用系统时间。
//
//		父类计时器修改后应用程序应该调用 reset 来重启计时器，否则计时器内部状态是错的。
//	*/
//	void setParentTimer(Timer *timer) {
//		parentTimer = timer;
//	}
//
//protected:
//
//	void update(Real parentTime) {
//		Real cur;
//		if (pauseCount)
//			// the timer is paused. do not tick more.
//			cur = curTime;
//		else
//			cur = parentTime - baseTime - totalPausedTime;
//
//		ooBreakAssert(cur >= curTime); // "Time is a bird for ever on the wing. - T.W.Roberson."
//
//		lastTime = curTime;
//		curTime = cur;
//
//		// DO NOT USE: deltaTime = curTime - lastTime, it cause sum(DeltaTime) != curTime for precision reason.
//		deltaTime = curTime - totalDeltaTime;
//		totalDeltaTime += deltaTime;
//
//		frameCount = getNextFrameCount();
//
//		fpsRecorder.update(curTime);
//	}
//
//	Real getParentTime() const {
//		return parentTimer ? parentTimer->getCurTime() : Utility::GetTime();
//	}
//
//	//Uint nextFrameCount() { frameCount = getNextFrameCount(); return frameCount; } // avoid overflow, clamp in[0, 0xefffffff]
//
//	// parent time.
//	Timer *parentTimer;
//	Real baseTime;
//
//	int pauseCount;
//	Real startPauseTime;
//	Real totalPausedTime;
//	Real totalDeltaTime;
//
//	// local time.
//	Real curTime;
//	Real lastTime;
//	Real deltaTime;
//	
//	Uint frameCount;
//
//	FPSRecorder fpsRecorder;
//};
//
//class OO_API SetupLocal
//{
//public:
//	SetupLocal(const char* localeName = NULL);
//	~SetupLocal();
//private:
//	String oldLocName;
//	SetupLocal(const SetupLocal&) {}
//	SetupLocal*	operator = (const SetupLocal& right){return NULL;}
//};

OO_NAMESPACE_END

#endif // #ifndef OO_UTILITY_H_
