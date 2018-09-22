/*
	Unicode support for tinyxml.
	Original file by Bin Wang, skull_wang@hotmail.com.

	This software is provided 'as-is', without any express or implied 
	warranty. In no event will the authors be held liable for any 
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any 
	purpose, including commercial applications, and to alter it and 
	redistribute it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must 
	not claim that you wrote the original software. If you use this 
	software in a product, an acknowledgment in the product documentation 
	would be appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any source 
	distribution.

	Revision History:
		* Created by Bin Wang, 7/3/03.

*/
#ifndef TIXML_UNICODE_H_

// modified by Bin Wang for mbcs support.
#if WIN32
#include <mbstring.h>
#endif

// User config
// Toggle unicode support.
#define TIXML_UNICODE 1

#if TIXML_UNICODE

#if WIN32
#	define ti_isspace _ismbcspace
#else
#	define ti_isspace isspace
#endif
//#	define _TI_T(x) L ## x
//	typedef TiChar wchar_t

#else

#	define ti_isspace isspace
//#	define _TI_T(x) x
//	typedef TiChar char

#endif

#endif