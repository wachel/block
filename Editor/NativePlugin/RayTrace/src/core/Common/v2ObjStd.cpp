#include "v2ObjStd.h"

void logWarning( LOG_BOOL_TYPE bOpen, const char *formats, ... )
{
	// 优性能，提前判断
	if (!bOpen)
		return;

	//Log *log = Object_getDefaultLog();
	//if (!log || log->isIgnore(Log::LV_Warning))
	//	return;

	//va_list list;
	//va_start( list, formats);

	//log->vlog(Log::LV_Warning, formats, list);

	//va_end( list );
}

//void logWarning( LOG_BOOL_TYPE bOpen,const String &formats )
//{
//	//logWarning(bOpen, formats.c_str());
//}

void logError( LOG_BOOL_TYPE bOpen, const char *formats, ... )
{

}

//void logError( LOG_BOOL_TYPE bOpen,const String &formats )
//{
//
//}

void logInfo( LOG_BOOL_TYPE bOpen, const char *formats, ... )
{

}

void logNotice( LOG_BOOL_TYPE bOpen, const char *formats, ... )
{

}
