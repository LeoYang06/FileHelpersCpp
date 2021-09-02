#include "pch.h"
#include <fstream>
#include <iostream>
#include "FileSteamEngineBase.h"

using namespace file_helpers_cpp;

/// <summary>
/// 统计一个文件的行数。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="error">错误信息。</param>
/// <returns>文件中所有文本的行数。</returns>
int FileSteamEngineBase::CountLines(const std::string& path, std::error_code error) const
{
	int line_count = 0;
	std::ifstream infile(path.c_str(), std::ios::in);

	if (!infile.good())
	{
		error = std::make_error_code(std::errc::bad_file_descriptor);
		return -1;
	}

	line_count = std::count(std::istreambuf_iterator<char>(infile),
		std::istreambuf_iterator<char>(), '\n');

	infile.close();
	return line_count;
}

/// <summary>
/// 打开一个文本文件，将文件中的所有文本读取到一个字符串中，然后关闭此文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_all_text">包含文件中所有文本的字符串。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool FileSteamEngineBase::ReadAllText(const std::string& path, std::string& out_all_text, std::error_code error)  const
{
	std::ifstream infile;
	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		error = std::make_error_code(std::errc::bad_file_descriptor);
		return false;
	}
	infile >> out_all_text;
	infile.close();
	return true;
}

/// <summary>
/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool FileSteamEngineBase::ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error) const
{
	const int line_count = CountLines(path, error);
	out_all_lines.reserve(line_count);
	std::ifstream infile;
	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		error = std::make_error_code(std::errc::bad_file_descriptor);
		return false;
	}
	std::string str_line;
	while (std::getline(infile, str_line))
	{
		// 空行判断
		if (str_line == "\r\n")
		{
			str_line.clear();
			continue;
		}

		out_all_lines.push_back(str_line);
		str_line.clear();
	}
	infile.close();
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
bool FileSteamEngineBase::ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error, int start_line, int max_records)  const
{
	out_all_lines.reserve(max_records);
	std::ifstream infile;
	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		error = std::make_error_code(std::errc::bad_file_descriptor);
		return false;
	}

	std::string str_line;
	int readed_line_number = 1;

	while (getline(infile, str_line))
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

	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入指定的字符串，然后关闭文件。 如果目标文件已存在，则覆盖该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的字符串。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool FileSteamEngineBase::WriteAllText(const std::string& path, const std::string& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	PreAllocateFile(path);

	std::ofstream outfile(path);
	outfile << contents << std::endl;
	outfile.close();
	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入一个字符串集合，然后关闭该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入到文件中的行。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool FileSteamEngineBase::WriteAllLines(const std::string& path, const std::vector<std::string>& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	PreAllocateFile(path);

	std::ofstream outfile(path, std::ios::app);
	for (const auto& line : contents)
	{
		outfile << line << std::endl;
	}
	outfile.close();
	return true;
}