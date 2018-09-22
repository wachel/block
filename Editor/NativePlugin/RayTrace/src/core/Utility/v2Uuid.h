#pragma once

#include "v2String.h"

OO_NAMESPACE_BEGIN

static const int UUID_BYTES = 16;

/** 全局唯一标识符(Universally Unique Identifier)
*/
struct OO_API Uuid
{
public:
	union {
		Byte data[UUID_BYTES];
		Dword dwData[4];
	};

	//@{ 
	/// 常量。
	static const Uuid ZERO;
	//@}

	//@{ 
	/// 构造函数。
	Uuid() {}

	Uuid(const Uuid &in) {
		dwData[0] = in.dwData[0];
		dwData[1] = in.dwData[1];
		dwData[2] = in.dwData[2];
		dwData[3] = in.dwData[3];
	}

	/** 从字符串产生 UUID。
		@param str 一个表示UUID的字符串，如："E6651A30-7BF3-4507-BA46-42926E0640DC"

		字符串大小写无关，分隔符任意。
	*/
	Uuid(const String& in) {
		fromString(in);
	}

	/** 从字符串产生 UUID。
		@param str 一个表示UUID的字符串，如："E6651A30-7BF3-4507-BA46-42926E0640DC"

		字符串大小写无关，分隔符任意。
	*/
	Uuid(const char* in) {
		fromString(in);
	}
	//@}

	bool operator == (const Uuid& rhs) const {
		return dwData[0] == rhs.dwData[0] && dwData[1] == rhs.dwData[1] && dwData[2] == rhs.dwData[2] && dwData[3] == rhs.dwData[3];
	}

	bool operator != (const Uuid& rhs) const {
		return !(*this == rhs);
	}

	bool operator < (const Uuid& rhs) const {
		for (int i = 0; i < 3; i++) {
			if (dwData[i] != rhs.dwData[i])
				return dwData[i] < rhs.dwData[i];
		}
		return dwData[3] < rhs.dwData[3];
	}

	bool operator > (const Uuid& rhs) const {
		for (int i = 0; i < 3; i++) {
			if (dwData[i] != rhs.dwData[i])
				return dwData[i] > rhs.dwData[i];
		}
		return dwData[3] > rhs.dwData[3];
	}

	/** 从字符串产生 UUID。
		@param str 一个表示UUID的字符串，如："E6651A30-7BF3-4507-BA46-42926E0640DC"
		
		字符串大小写无关，分隔符任意。
	*/
	bool fromString(const char* str);
	/** 从字符串产生 UUID。
		@param str 一个表示UUID的字符串，如："E6651A30-7BF3-4507-BA46-42926E0640DC"

		字符串大小写无关，分隔符任意。
	*/
	bool fromString(const String &str) {
		return fromString(str.c_str());
	}

	/** 返回表示 UUID 的字符串。
		@return 字符串。

		返回的字符串是大小，如："E6651A30-7BF3-4507-BA46-42926E0640DC"
	*/
	String toString() const;

	void generate();


};

COMPILER_ASSERT(sizeof(Uuid) == UUID_BYTES);

OO_NAMESPACE_END
