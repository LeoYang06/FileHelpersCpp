#include "pch.h"
#include <fstream>
#include "FileEngineBase.h"
#include "StringConverter.h"

using namespace file_helpers_cpp;

/// <summary>
/// 判断文件是否存在。
/// </summary>
/// <param name="file_name">文件名称。</param>
/// <returns>是否存在。</returns>
bool FileEngineBase::FileExists(const std::string& file_name) const
{
	struct stat buffer;
	return (stat(file_name.c_str(), &buffer) == 0);
}

/// <summary>
/// 按指定大小初始化文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="size">分配的文件大小。</param>
void FileEngineBase::AllocateFile(const std::string& path, const long size) const
{
	std::ofstream outfile(path);
	if (size > 0)
	{
		const std::string s(size, '\0');
		outfile << s;
	}
	outfile.close();
}

/// <summary>
/// 按指定大小为文件预分配磁盘空间。内存映射文件时建议使用。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="error">错误信息。</param>
/// <param name="size">预分配的文件大小。</param>
/// <returns>预分配文件是否成功。</returns>
bool FileEngineBase::PreAllocateFile(const std::string& path, std::string& error, const long size) const
{
	if (path.empty())
	{
		return false;
	}
	const std::wstring wstr_path = ToWString(path);
	const LPCWSTR file_path = wstr_path.c_str();
	// create file
	const HANDLE file_handle = CreateFile(file_path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		error = StringFormat("error CreateFile( %s ) failed. gle = %u", ToString(file_path).c_str(), GetLastError());
		return false;
	}

	LARGE_INTEGER file_size = {0};
	//file_size.LowPart = 0;
	//file_size.HighPart = 1;
	file_size.QuadPart = size;

	if (!SetFilePointerEx(file_handle, file_size, nullptr, FILE_BEGIN))
	{
		error = StringFormat("error SetFilePointerEx() failed. gle = %u", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	SetEndOfFile(file_handle);
	CloseHandle(file_handle);
	return true;

	//SetFilePointer(file_handle , size, nullptr, FILE_BEGIN);
	//SetEndOfFile(file_handle );
	//CloseHandle(file_handle );
}
