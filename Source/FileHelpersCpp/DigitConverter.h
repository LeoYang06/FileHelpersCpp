#pragma once
#include <string>
#include <atlstr.h>

using namespace std;

inline int ToInt(string& value)
{
	const int num = atoi(value.c_str());
	return num;
}

inline double ToDouble(string& value)
{
	const double num = atof(value.c_str());
	return num;
}

inline float ToFloat(string& value)
{
	const float num = atof(value.c_str());
	return num;
}

inline long ToLong(string& value)
{
	const long num = atol(value.c_str());
	return num;
}

inline long long ToLonglong(string& value)
{
	const long long num = atoll(value.c_str());
	return num;
}

inline int ToInt(const CString& value)
{
	const int num = _wtoi(value);
	return num;
}

inline double ToDouble(const CString& value)
{
	const double num = _wtof(value);
	return num;
}

inline float ToFloat(const CString& value)
{
	const float num = _wtof(value);
	return num;
}

inline long ToLong(const CString& value)
{
	const long num = _wtol(value);
	return num;
}

inline long long ToLonglong(const CString& value)
{
	const long long num = _wtoll(value);
	return num;
}
