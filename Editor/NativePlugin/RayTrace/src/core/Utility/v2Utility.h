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
///** �����Ĺ����ࡣ
//*/
//class OO_API Utility
//{
//public:
//
//	/** ����ϵͳʱ�䣬���롣
//		
//		Win32ƽ̨�·���Windows�����������ʱ�䣬���򷵻ص��ý��̵�ʱ�䡣
//	*/
//	static Real getTime();
//
//	/** ��������ȱʡ�ļ�ʱ���� */
//	static Timer& getTimer();
//
//	// �������������淶���� Utility::getTime��
//	static Real GetTime() {
//		return getTime();
//	}
//
//// ����·����صĺ����������ˣ����� PathUtility ��
//
//	// ��������PathUtility::getFileLength
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
///** �����ļ�·���õĹ����ࡣ
//
//	������Ҫ��·�����ǡ�/�������ָ��·������ toEnginePath ����ת��"\"��"/"��
//*/
//class OO_API PathUtility
//{
//public:
//	// ȡ��ִ���ļ�·����eg. "d:/Origo/bin".
//	static String getModulePath();
//
//	// eg. "D:\AaAa\BbBb" - > "d:/aaaa/bbbb"
//	// �����ڲ�ʹ�ã�һ�㲻Ҫת����Сд��
//	static String checkPath(const String &path);
//
//	// eg. "D:\AaAa\BbBb" - > "D:/AaAa/BbBb"
//	// ת���������ڲ��õ�·������"/"�ָ�"��
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
//	/** ȡ���ļ����ȡ�
//		
//		@param pathname ������һ������·����
//	*/
//	static Uint getFileLength(const String& pathname);
//
//	/** ȡ���д�Сд���ļ����֡�
//		@param path �ļ�·����
//		@return �д�Сд���ļ����֡�
//
//		�ú���ͨ�����Ҵ����ļ�������һ���ļ��ڴ����ϴ���Сд��Ϣ�����֡�
//		path��ָ���ļ�����棬���򷵻�""��
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
///** ��ʱ����
//*/
//class OO_API Timer
//{
//public:
//	Timer() : parentTimer(0) { reset(); }
//	Timer(Timer *parent) : parentTimer(parent) { reset(); }
//
//	/** ȡ�õ�ǰʱ�䡣��λ���롣
//	*/
//	Real getCurTime() const {
//		return curTime;
//	}
//	/** ȡ�õ�ǰ֡��ͨ�� @ref getCurTime ���أ���ǰһ֡��ʱ�䡣
//		
//		@return ʱ�䡣
//	*/
//	Real getLastTime() const {
//		return lastTime;
//	}
//	/** ȡ��������ʱ����λ���롣
//		@return deltaTime ʱ��ֻ��֤ >= 0���п��ܷ���0��
//	*/
//	Real getDeltaTime() const {
//		return deltaTime;
//	}
//
//	/** ���¼�ʱ����
//		Ӧ�ó��������� update ��ʱ���Ż���µ�ǰʱ�����ݣ��������һ
//		������ѭ����ÿ��ѭ������һ�Ρ�
//	*/
//	void update(){
//		update(getParentTime());
//	}
//
//	/** �����ʱ����
//		
//		@param cur ��ǰʱ�䣬������ @ref getCurTime �ķ���ֵ��
//		@param delta ʱ�������� @ref getDeltaTime �ķ���ֵ��
//		@param frame �ۼ�֡���������� @ref getFrameCount �ķ���ֵ��
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
//	/** ��ͣ��
//
//		@return ��ͣ������
//
//		��ʱ������һ @b ��ͣ���� �����ж��Ƿ�������ͣ����ÿ�ε��� @ref pause
//		�� @ref resume ��ͣ�������1�ͼ�1������ͣ����Ϊ0ʱ��ʱ���š�������ͣ����
//	*/
//	int pause() {
//		pauseCount ++;
//		startPauseTime = getParentTime();
//		return pauseCount;
//	}
//
//	/** ������
//
//		@return ��ͣ������
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
//	/** �Ƿ���ͣ��
//		@return �Ƿ���ͣ��
//	*/
//	bool isPaused() const {
//		return pauseCount > 0;
//	}
//	/** ȡ���ۼ�֡����
//		
//		@return �ۼ�֡����[0, 0xefffffff]��
//
//		ÿ�ε��� update ʱ�ۼ�֡���ͻ��1���������ֵ����ٴ�0��ʼ��
//	*/
//	Uint getFrameCount() const { return frameCount; }
//
//	
//	/** ȡ����һ�� update ����ۼ�֡����
//		
//		@return �ۼ�֡����
//		@see getFrameCount
//	*/
//	Uint getNextFrameCount() const { 
//		Uint next = frameCount + 1; next &= 0xefffffff; return next; // avoid overflow, clamp in[0, 0xefffffff]
//	}
//
//	/** ȡ��FPS��
//		
//		@return ÿ��ƽ��֡����
//	*/
//	Real getFPS() const {
//		return fpsRecorder.getFPS();
//	}
//
//	/** ȡ�ø����ʱ����
//		
//		@return �����ʱ���� NULL ��ʾû�и����ʱ������ϵͳʱ�䡣
//	*/
//	Timer* getParentTimer() {
//		return parentTimer;
//	}
//
//	/** ���ø����ʱ����
//
//		@param timer �����ʱ���� NULL ��ʾû�и����ʱ������ϵͳʱ�䡣
//
//		�����ʱ���޸ĺ�Ӧ�ó���Ӧ�õ��� reset ��������ʱ���������ʱ���ڲ�״̬�Ǵ�ġ�
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
