// This file is part of Origo Engine.
// Copyright (C), Beijing Tuyasoft Co., Ltd.
//#include "v2Standard.h"
#include "v2Uuid.h"

//#include "windows.h" // ::UuidCreate...

OO_NAMESPACE_BEGIN

const Uuid Uuid::ZERO("");

bool Uuid::fromString(const char* str) 
{
	if (!str) {
		*this = ZERO;
		return false;
	}
	
	if (str[0] == '\0') {
		*this = ZERO;
		return true;
	}
		
	int pos = 0;
	for (int i = 0; i < UUID_BYTES; i++) {
		if (i == 4 || i ==	6 || i == 8 || i == 10) {
			pos ++; // Ìø¹ý·Ö¸îÏß
		}

		Byte high;
		if (str[pos] >= '0' && str[pos] <= '9')
			high = str[pos] - '0';
		else if (str[pos] >= 'a' && str[pos] <= 'f')
			high = 10 + str[pos] - 'a';
		else if (str[pos] >= 'A' && str[pos] <= 'F')
			high = 10 + str[pos] - 'A';
		else {
			*this = ZERO;
			return false;
		}

		pos ++;
		Byte low;
		if (str[pos] >= '0' && str[pos] <= '9')
			low = str[pos] - '0';
		else if (str[pos] >= 'a' && str[pos] <= 'f')
			low = 10 + str[pos] - 'a';
		else if (str[pos] >= 'A' && str[pos] <= 'F')
			low = 10 + str[pos] - 'A';
		else {
			*this = ZERO;
			return false;
		}
		pos ++;

		data[i] = (high << 4) | low;
	}
	
	return true;
}

String Uuid::toString() const
{
	// load appropriate formatting string
	String strFormat;
	strFormat = uString::format("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		data[0], data[1], data[2], data[3], 
		data[4], data[5], data[6], data[7], 
		data[8], data[9], data[10], data[11], 
		data[12], data[13], data[14], data[15]);
	return strFormat;
}

void Uuid::generate()
{
	// create random GUID using UuidCreate so that we 
	// can check for more error codes
	//GUID guid = GUID_NULL;
	//HRESULT hr = ::UuidCreate(&guid);
	//if (FAILED(hr)) {
	//	ooAssert(false);
	//	*this = ZERO;
	//}
	//else {
	//	memcpy(data, &guid, sizeof(guid));
	//}

}

OO_NAMESPACE_END