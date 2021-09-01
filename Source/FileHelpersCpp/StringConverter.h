#pragma once
#include <atlconv.h>
#include <string>
#include <atlstr.h>
#include <sstream>

using namespace std;

/// <summary>
/// 转换为字符串类型。
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline string ToString(double value)
{
	auto str = to_string(value);
	return str;
}

/// <summary>
/// 转换为字符串类型。
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline string ToString(int value)
{
	auto str = to_string(value);
	return str;
}

/// <summary>
/// 转换为字符串类型。
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline string ToString(float value)
{
	auto str = to_string(value);
	return str;
}

/// <summary>
/// 转换为字符串类型。
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline string ToString(long value)
{
	auto str = to_string(value);
	return str;
}

/// <summary>
/// 转换为字符串类型。
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
inline string ToString(const CString& value)
{
	USES_CONVERSION;
	const auto* const str = T2A(value);
	return str;
}

/// <summary>
/// 转换为字符串类型的泛型函数。
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="value"></param>
/// <returns></returns>
template <typename T>
string ToString(const T& value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

inline std::wstring ToWString(const std::string& str)
{
	//int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	//wchar_t* wide = new wchar_t[len + 1];
	//memset(wide, '\0', sizeof(wchar_t) * (len + 1));
	//MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, len);
	//std::wstring w_str(wide);
	//delete[] wide;
	//return w_str;
	
	if (str.empty())
		return{};
	const int s_length = static_cast<int>(str.length());
	auto buffer = std::vector<wchar_t>(s_length);
	const auto wide_char_count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), s_length, buffer.data(), s_length);
	return std::wstring(buffer.data(), wide_char_count);
}

inline std::string ToString(const std::wstring& wstr)
{
	//int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
	//char* buffer = new char[len + 1];
	//memset(buffer, '\0', sizeof(char) * (len + 1));
	//WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer, len, nullptr, nullptr);
	//std::string result(buffer);
	//delete[] buffer;
	//return result;
	
	if (wstr.empty())
		return{};
	const int s_length = static_cast<int>(wstr.length());
	auto buffer = std::vector<char>(s_length);
	const auto char_count=WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), s_length, buffer.data(), s_length, nullptr, nullptr);
	std::string result(buffer.data(), char_count);
	return result;
}

/// <summary>
/// 字符串格式化。
/// </summary>
/// <typeparam name="...Args">模板参数。</typeparam>
/// <param name="format">待格式化字符串。</param>
/// <param name="...args">可变数量参数。</param>
/// <returns>格式化后的字符串。</returns>
template <typename ... Args>
string StringFormat(const std::string& format, Args ... args)
{
	const size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	const std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
