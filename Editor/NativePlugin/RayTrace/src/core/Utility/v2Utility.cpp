// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2Utility.h"

//#include <direct.h>
//#include <io.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//
//// Windows Platform
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#	include "windows.h"
//#	include <windowsx.h>
//#	include "mmsystem.h"
//#	include "tchar.h"
//#	pragma comment ( lib, "winmm" )
//#endif
//
//OO_NAMESPACE_BEGIN
//
//Real Utility::getTime()
//{
//	Real cur;
//
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	// defaults are for timeGetTime()
//	
//	// 注意：QueryPerformanceFrequency结果会顺着系统运行时间的延长而改变，例如运行1个月后精度可能变为0.5s。
//	//		 所以需要每次都查询，防止长时间运行的服务器程序出现Bug。
//
//	// query time
//	LARGE_INTEGER freq;
//	LARGE_INTEGER counter;
//	if ( ::QueryPerformanceFrequency( &freq ) && ::QueryPerformanceCounter( &counter ) )
//	{
//		Real seconds = static_cast<Real> ( counter.QuadPart);
//
//		Real sTimerSecondsConversion = 1.f / freq.QuadPart;
//		cur = (Real)( seconds * sTimerSecondsConversion );
//	}
//	else
//	{
//		DWORD seconds = ::timeGetTime();
//		cur = (Real)( seconds * 0.001f);
//	}
//	
//#else // #if OO_PLATFORM == OO_PLATFORM_WIN32
//	
//	cur = (Real)clock() / CLOCKS_PER_SEC;
//
//#endif
//
//	// avoid the case of Utility::GetTime() returning the time which may smaller the last one, really weired.
//    static Real last = 0.f;
//
//	if (last > cur) {
//		return last;
//	}
//	else {
//		last = cur;
//		return cur;
//	}
//}
//
//Timer& Utility::getTimer()
//{
//	static Timer timer;
//	return timer;
//}
//
//String Utility::GetAbsolutePath(const String& path, const String &rootDir)
//{
//	if (path.find(":") != String::npos) {
//		// abs path already.
//		return CheckPath(path);
//	}
//
//	char savedWorkDir[_MAX_PATH], absPath[_MAX_PATH];
//
//	getcwd(savedWorkDir, _MAX_PATH);
//
//	// change to root dir.
//	if (chdir(rootDir.c_str()) != 0)
//		ooBreakAssert(false);
//
//	// change to relative path.
//	if (chdir(path.c_str()) != 0)
//		ooBreakAssert(false);
//
//	getcwd(absPath, _MAX_PATH);
//	
//	// restores the work directory.
//	chdir(savedWorkDir);
//
//	return CheckPath(absPath);
//}
//
//String Utility::GetRelativePath(const String& path, const String &rootDir)
//{
//	ooBreakAssert(path.size() >= rootDir.size());
//
//	String str = uString::right(path, path.size() - rootDir.size());
//
//	return str;
//}
//
//String Utility::GetAbsolutePathName(const String& pathname)
//{
//	String transed = uString::replace(pathname, '/', '\\');
//
//	String path, name;
//	int pos;
//	if ((pos = transed.find_last_of('\\')) != String::npos) {
//		path = transed.substr(0, pos);
//		name = transed.substr(pos + 1);
//	}
//	else if (uString::right(transed, 2) == "..") {
//		path = transed;
//		name = "";
//	}
//	else {
//		path = ".";
//		name = transed;
//	}
//
//	char savedWorkDir[_MAX_PATH], absPath[_MAX_PATH];
//	
//	getcwd(savedWorkDir, _MAX_PATH);
//	if (chdir(path.c_str()) == -1)
//		//ooExcept(Exception::errGeneric, "Try to change directory failed!");
//		ooBreakAssert(false);
//
//	getcwd(absPath, _MAX_PATH);
//	
//	// restores the work directory.
//	chdir(savedWorkDir);
//	
//	return String(absPath) + "\\" + name;
//}
//
//Uint Utility::GetFileLength(const String& pathname)
//{
//	return PathUtility::getFileLength(pathname);
//}
//
//String Utility::GetPathFromPathName(const String& pathname)
//{
//	ooBreakAssert(pathname.length() < MAX_PATH );
//
//	if (pathname.empty())
//		return "";
//
//	String transed = uString::replace(pathname, '/', '\\');
//
//	char path[MAX_PATH];
//
//	strcpy(path, const_cast<char*>(transed.c_str()));
//
//	char* pch = path + strlen(path) - 1;
//	while('\\' != *pch && pch > path )
//		pch--;
//	if ( '\\' == *pch )
//		*(pch+1) = '\0';
//
//	return String(path);
//}
//
//String Utility::GetNameFromPathName(const String& pathname)
//{
//	ooBreakAssert(pathname.length() < MAX_PATH );
//
//	if (pathname.empty())
//		return "";
//
//	String transed = uString::replace(pathname, '/', '\\');
//
//	const char* pch = transed.c_str() + strlen(transed.c_str()) - 1;
//	while(pch > transed.c_str() && *pch != '\\')
//		pch--;
//	if ( '\\' == *pch )
//		pch++;
//
//	return String(pch);
//}
//
//
//String Utility::GetFileNameExtension(const String& filename)
//{
//	String::size_type pos = filename.find_last_of('.');
//	if (pos == String::npos)
//		return "";
//	else {
//		String ext = uString::right(filename, filename.size() - pos - 1);
//		ext = uString::uppercase(ext);
//		return ext;
//	}
//}
//
//String Utility::GetFileNameWithoutExtension(const String& filename)
//{
//	String name = GetNameFromPathName(filename);
//	String::size_type pos = name.find_last_of('.');
//	if (pos != String::npos)
//		return uString::left(name, pos);
//	else
//		return name;
//}
//
//String Utility::RemoveExtension(const String& filename)
//{
//	String::size_type pos = filename.find_last_of('.');
//
//	if (pos != String::npos) {
//		String str;
//		str.assign(filename.begin(), filename.begin() + pos);
//		return str;
//	}
//	else
//		return filename;
//}
//
//String Utility::GetModulePath()
//{
//	String modulePath;
//
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	HMODULE hModule = GetModuleHandle("ooCore.dll");
//	char szPathName[MAX_PATH];
//	GetModuleFileName(hModule, szPathName, MAX_PATH);
//	modulePath = Utility::GetPathFromPathName(szPathName);
//#else
//#	error "not yet finish!"
//#endif
//
//	return modulePath;
//}
//
//String Utility::CheckPath(const String &path)
//{
//	String validPath = path;
//
//	if (!validPath.empty()) {
//		validPath = uString::lowercase(validPath);
//		validPath = uString::replace(validPath, '/', '\\');
//
//        if (validPath[validPath.size()-1] != '\\')
//			validPath+= "\\";
//	}
//
//	return validPath;
//}
//
//String PathUtility::getModulePath()
//{
//	String modulePath;
//
//#if OO_PLATFORM == OO_PLATFORM_WIN32
//	HMODULE hModule = GetModuleHandle("ooCore.dll");
//	char szPathName[MAX_PATH];
//	::GetModuleFileName(hModule, szPathName, MAX_PATH);
//	modulePath = PathUtility::removeName(checkPath(szPathName));
//#else
//#	error "not yet finish!"
//#endif
//
//	return modulePath;
//}
//
//String PathUtility::checkPath(const String &path)
//{
//	String validPath = path;
//
//	if (!validPath.empty()) {
//		validPath = uString::lowercase(validPath);
//		validPath = uString::replace(validPath, '\\', '/'); // win32 path to engine path.
//	}
//
//	return validPath;
//}
//
//String PathUtility::removeName(const String& path)
//{
//	int pos = path.find_last_of('/');
//	if (pos != String::npos)
//		return String(path.begin(), path.begin() + pos);
//	else
//		return path;
//}
//
//String PathUtility::removeLastSlash(const String& path)
//{
//	if (!path.empty() && *(--path.end()) == '/')
//		return String(path.begin(), path.end() - 1);
//	else
//		return path;
//}
//
//String PathUtility::getExtension(const String& path)
//{
//	String::size_type pos = path.find_last_of('.');
//
//	if (pos != String::npos)
//		return String(path.begin() + pos + 1, path.end());
//	else
//		return "";
//}
//
//String PathUtility::removeExtension(const String& path)
//{
//	String::size_type pos = path.find_last_of('.');
//
//	if (pos != String::npos)
//		return String(path.begin(), path.begin() + pos);
//	else
//		return path;
//}
//
//String PathUtility::getName(const String& path)
//{
//	if (path.empty())
//		return "";
//
//	const char* pch = path.c_str() + strlen(path.c_str()) - 1;
//	while(pch > path.c_str() && *pch != '/')
//		pch--;
//	if ( '/' == *pch )
//		pch++;
//
//	return String(pch);
//}
//
//String PathUtility::getNameNoExt(const String& path)
//{
//	String name = getName(path);
//	String::size_type pos = name.find_last_of('.');
//	if (pos != String::npos)
//		return uString::left(name, pos);
//	else
//		return name;
//}
//
//bool PathUtility::getDirsInDir(StringVector &names, const String& dir)
//{
//	names.clear();
//
//	char m_savedWorkDir[_MAX_PATH];
//	getcwd(m_savedWorkDir, _MAX_PATH);
//
//	if (chdir(dir.c_str()) == -1)
//		return false;
//
//	long lHandle, res;
//	struct _finddata_t tagData;   
//
//	lHandle = _findfirst("*.*", &tagData);
//	res = 0;
//	while (lHandle != -1 && res != -1)
//	{
//		// note: == 0 skips "." and "..".
//		if ((tagData.attrib & _A_SUBDIR) && strcmp(tagData.name,".")!=0 &&  strcmp(tagData.name,"..")!=0)
//			names.push_back(tagData.name);
//
//		res = _findnext( lHandle, &tagData );
//	}
//	if(lHandle != -1)
//		_findclose(lHandle);
//
//	//unsetWorkDir();
//	if (chdir(m_savedWorkDir) == -1)
//		return false;
//
//	return true;
//}
//
//bool PathUtility::getFilesInDir(StringVector &names, const String& dir)
//{
//	names.clear();
//
//	char m_savedWorkDir[_MAX_PATH];
//	getcwd(m_savedWorkDir, _MAX_PATH);
//	if (chdir(dir.c_str()) == -1)
//		return false;
//
//	long lHandle, res;
//	struct _finddata_t tagData;   
//
//	lHandle = _findfirst("*.*", &tagData);
//	res = 0;
//	while (lHandle != -1 && res != -1)
//	{
//		if (!(tagData.attrib & _A_SUBDIR))
//			names.push_back(tagData.name);
//
//		res = _findnext( lHandle, &tagData );
//	}
//	if(lHandle != -1)
//		_findclose(lHandle);
//
//	if (chdir(m_savedWorkDir) == -1)
//		return false;
//
//	return true;
//}
//
//bool PathUtility::isDir(const String& dir)
//{
//	char savedWorkDir[_MAX_PATH];
//	getcwd(savedWorkDir, _MAX_PATH);
//
//	if (chdir(dir.c_str()) == -1)
//		return false;
//	else {
//		chdir(savedWorkDir);
//		return true;
//	}
//}
//
//bool PathUtility::makeDir(const String& dir, bool recursive)
//{
//	if (!recursive) {
//		mkdir(dir.c_str());
//		return isDir(dir.c_str());
//	}
//	else {
//		String dir2 = uString::replace(dir, '\\', '/'); // win32 path to engine path.
//		dir2 = PathUtility::removeLastSlash(dir2) + "/";
//
//		size_t pos = dir2.find_first_of('/');
//		while (pos != String::npos) {
//			String cur(dir2.begin(), dir2.begin() + pos);
//			mkdir(cur.c_str());
//			if (isDir(cur.c_str()))
//				pos = dir2.find_first_of('/', pos + 1);
//			else
//				return false;
//		}
//		
//		return true;
//	}
//}
//
//
//#if	OO_PLATFORM == OO_PLATFORM_WIN32
//
//BOOL IsDots(const TCHAR* str) {
//    if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
//    return TRUE;
//}
//
//BOOL DeleteDirectory(const TCHAR* sPath) 
//{
//    HANDLE hFind;  // file handle
//
//    WIN32_FIND_DATA FindFileData;
//
//    TCHAR DirPath[MAX_PATH];
//    TCHAR FileName[MAX_PATH];
//
//    _tcscpy(DirPath,sPath);
//    _tcscat(DirPath,"\\*");    // searching all files
//
//    _tcscpy(FileName,sPath);
//    _tcscat(FileName,"\\");
//
//    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
//    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
//    _tcscpy(DirPath,FileName);
//        
//    bool bSearch = true;
//    while(bSearch) { // until we finds an entry
//        if(FindNextFile(hFind,&FindFileData)) {
//            if(IsDots(FindFileData.cFileName)) continue;
//            _tcscat(FileName,FindFileData.cFileName);
//            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//
//                // we have found a directory, recurse
//                if(!DeleteDirectory(FileName)) { 
//                    FindClose(hFind); 
//                    return FALSE; // directory couldn't be deleted
//                }
//                RemoveDirectory(FileName); // remove the empty directory
//                _tcscpy(FileName,DirPath);
//            }
//            else {
//                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
//                    _chmod(FileName, _S_IWRITE); // change read-only file mode
//                if(!DeleteFile(FileName)) {  // delete the file
//                    FindClose(hFind); 
//                    return FALSE; 
//                }                 
//                _tcscpy(FileName,DirPath);
//            }
//        }
//        else {
//            if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
//            bSearch = false;
//            else {
//                // some error occured, close the handle and return FALSE
//                FindClose(hFind); 
//                return FALSE;
//            }
//
//        }
//
//    }
//    FindClose(hFind);  // closing file handle
// 
//    return RemoveDirectory(sPath); // remove the empty directory
//
//}
//#endif
//
//bool PathUtility::deleteDir(const String &dir)
//{
//#if	OO_PLATFORM == OO_PLATFORM_WIN32
//	return DeleteDirectory(dir.c_str()) != 0;
//#else
//	OO_UNIMPLEMENTED_METHOD
//#endif
//}
//
//Uint PathUtility::getFileLength(const String& pathname)
//{
//	String transed = uString::replace(pathname, '/', '\\');
//
//	int fh;
//	Uint bytes = 0;
//	// Open a file 
//	if( (fh = _open( transed.c_str(), _O_BINARY | _O_RDONLY))  != -1 )
//	{
//		bytes = _filelength( fh );
//		_close( fh );
//	}
//	return bytes;
//}
//
//String PathUtility::getFileNameWithCase(const String &path)
//{
//	if (path.empty())
//		return "";
//	
//	long lHandle;
//	struct _finddata_t tagData;   
//
//	lHandle = _findfirst(path.c_str(), &tagData);
//	if (lHandle != -1) {
//		_findclose(lHandle);
//
//		return tagData.name;
//	}
//	else
//		return "";
//}
//
//
////////////////////////////////////////////////////////////////////////////
//
//FPSRecorder::FPSRecorder() :
//	fps(0),
//	timeFrameOld(0),
//	timeFpsOld(0),
//	tickCount(0)
//{
//}
//
//void FPSRecorder::update(Real timePos)
//{
//	++tickCount;
//
//	if (timeFrameOld == 0) {
//		// initializes it
//		timeFrameOld = timePos;
//		timeFpsOld = timeFrameOld;
//	}
//
//	Real timeNew = timePos;
//
//	// update fps counter per second.
//	if (timeNew - timeFpsOld > 1) {
//		fps = tickCount / (timeNew - timeFpsOld);
//		timeFpsOld = timeNew;
//
//		tickCount = 0;
//	}
//}
//
//
//SetupLocal::SetupLocal( const char* localeName /*= NULL*/ )
//{
//	oldLocName = setlocale(LC_ALL, NULL);
//	setlocale(LC_ALL,localeName);
//}
//SetupLocal::~SetupLocal()
//{
//	setlocale(LC_ALL,oldLocName.c_str());
//}
//OO_NAMESPACE_END