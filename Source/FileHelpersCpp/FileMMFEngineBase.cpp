#include "pch.h"
#include <fstream>
#include <system_error>
#include "mio.hpp"
#include "FileMMFEngineBase.h"

using namespace file_helpers_cpp;

/// <summary>
/// 统计一个文件的行数。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="error">错误信息。</param>
/// <returns>文件中所有文本的行数。</returns>
int FileMmfEngineBase::CountLines(const std::string& path, std::error_code error) const
{
	int line_count = 0;
	const mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	for (const auto& c : read_mmap)
	{
		if (c == '\n')
		{
			line_count++;
		}
	}
	return line_count;
}

/// <summary>
/// 打开一个文本文件，将文件中的所有文本读取到一个字符串中，然后关闭此文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_all_text">包含文件中所有文本的字符串。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool FileMmfEngineBase::ReadAllText(const std::string& path, std::string& out_all_text, std::error_code error) const
{
	const int line_count = CountLines(path, error);
	out_all_text.reserve(line_count * 1024);
	mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	if (error)
	{
		return false;
	}

	for (const auto& c : read_mmap)
	{
		out_all_text += c;
	}

	read_mmap.unmap();
	return true;
}

/// <summary>
/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool FileMmfEngineBase::ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error) const
{
	const int line_count = CountLines(path, error);
	out_all_lines.reserve(line_count);
	mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	if (error)
	{
		return false;
	}

	std::string str_line;

	for (const auto& c : read_mmap)
	{
		if (c == '\r')
		{
			continue;
		}
		if (c == '\n')
		{
			// 空行判断
			if (std::strlen(str_line.c_str()) == 0)
			{
				str_line.clear();
				continue;
			}

			out_all_lines.push_back(str_line);
			str_line.clear();
			continue;
		}
		str_line += c;
	}

	read_mmap.unmap();
	return true;
}

/// <summary>
/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
/// <param name="error">错误信息。</param>
/// <param name="start_line">开始读取文本的起始行。从1开始。</param>
/// <param name="max_records">要读取的最大记录数。Int32.MaxValue或-1表示读取所有记录。</param>
/// <returns>是否完成读取操作。</returns>
bool FileMmfEngineBase::ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error, int start_line, int max_records) const
{
	out_all_lines.reserve(max_records);
	mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	if (error)
	{
		return false;
	}

	std::string str_line;

	int readed_line_number = 1;

	for (const auto& c : read_mmap)
	{
		str_line += c;
		if (c == '\n')
		{
			// 非空行判断
			if (str_line != "\r\n" && readed_line_number >= start_line)
			{
				out_all_lines.push_back(str_line);
				if (out_all_lines.size() >= max_records)
				{
					break;
				}
			}

			str_line.clear();
			readed_line_number++;
		}
	}

	read_mmap.unmap();
	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入指定的字符串，然后关闭文件。 如果目标文件已存在，则覆盖该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的字符串。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool FileMmfEngineBase::WriteAllText(const std::string& path, const std::string& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	const long characters_size = contents.size();
	std::string error_msg;
	const bool result = PreAllocateFile(path, error_msg, characters_size);

	mio::mmap_sink rw_mmap = mio::make_mmap_sink(path, error);
	if (error)
	{
		return false;
	}

	int index = 0;
	for (const auto& c : contents)
	{
		rw_mmap[index] = c;
		index++;
	}
	rw_mmap.sync(error);
	rw_mmap.unmap();
	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入一个字符串集合，然后关闭该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入到文件中的行。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool FileMmfEngineBase::WriteAllLines(const std::string& path, const std::vector<std::string>& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	long characters_size = 0;
	for (const auto& line : contents)
	{
		characters_size += line.size();
		// 加上换行符(\r\n)长度
		characters_size += 2;
	}

	std::string error_msg;
	const bool result = PreAllocateFile(path, error_msg, characters_size);

	mio::mmap_sink rw_mmap = mio::make_mmap_sink(path, error);
	if (error)
	{
		return false;
	}
	int index = 0;
	for (const auto& line : contents)
	{
		for (const auto& c : line)
		{
			rw_mmap[index] = c;
			index++;
		}
		rw_mmap[index] = '\r';
		index++;
		rw_mmap[index] = '\n';
		index++;
	}
	rw_mmap.sync(error);
	rw_mmap.unmap();
	return true;
}
