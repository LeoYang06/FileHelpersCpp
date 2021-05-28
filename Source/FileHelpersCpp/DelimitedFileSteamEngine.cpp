#include "pch.h"
#include <fstream>
#include "DelimitedFileSteamEngine.h"
#include "StringUtils.h"

using namespace file_helpers_cpp;

/// <summary>
/// 有参构造函数。
/// </summary>
/// <param name="delimiter"></param>
DelimitedFileSteamEngine::DelimitedFileSteamEngine(const std::string& delimiter)
{
	this->delimiter = delimiter;
}

/// <summary>
/// 打开一个文本文件，将文件中的所有文本读取到一个字符串类型的二维向量，然后关闭此文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_string_vector">包含文件所有行的字符串类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool DelimitedFileSteamEngine::ReadFileAsStringVector(const std::string& path, std::vector<std::vector<std::string>>& out_string_vector, std::error_code error) const
{
	//const int line_count = FileSteamEngineBase::CountLines(path, error);
	//out_string_vector.reserve(line_count);

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
		if (std::strlen(str_line.c_str()) == 0)
		{
			str_line.clear();
			continue;
		}

		std::vector<std::string> str_fields = Split(str_line, this->delimiter, true);
		out_string_vector.push_back(str_fields);
		str_line.clear();
	}

	return true;
}

/// <summary>
/// 打开一个文本文件，将文件中的所有文本读取到一个字符串类型的二维向量，然后关闭此文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_double_vector">包含文件所有行的double类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool DelimitedFileSteamEngine::ReadFileAsDoubleVector(const std::string& path, std::vector<std::vector<double>>& out_double_vector, std::error_code error) const
{
	//const int line_count = FileSteamEngineBase::CountLines(path, error);
	//out_double_vector.reserve(line_count);

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
		if (std::strlen(str_line.c_str()) == 0)
		{
			str_line.clear();
			continue;
		}

		std::vector<double> double_fields = SplitIntoDouble(str_line, this->delimiter, true);
		out_double_vector.push_back(double_fields);
		str_line.clear();
	}

	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入一个字符串类型的二维向量的集合，然后关闭该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的字符串类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool DelimitedFileSteamEngine::WriteAllStringVector(const std::string& path, const std::vector<std::vector<std::string>>& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	AllocateFile(path);

	std::ofstream outfile(path, std::ios::app);
	for (const auto& line_vector : contents)
	{
		int index = 0;
		for (auto field : line_vector)
		{
			index++;
			outfile << field;
			if (index < line_vector.size())
				outfile << delimiter;
		}
		outfile << "\n";
	}
	outfile.close();
	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入一个double类型的二维向量的集合，然后关闭该文件。文件存在则覆盖。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的double类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool DelimitedFileSteamEngine::WriteAllDoubleVector(const std::string& path, const std::vector<std::vector<double>>& contents, std::error_code error) const
{
	// 始终覆盖创建新文件。
	AllocateFile(path);

	std::ofstream outfile(path, std::ios::app);
	for (const auto& line_vector : contents)
	{
		int index = 0;
		for (auto field : line_vector)
		{
			index++;
			outfile << std::to_string(field);
			if (index < line_vector.size())
				outfile << delimiter;
		}
		outfile << "\n";
	}
	outfile.close();
	return true;
}
