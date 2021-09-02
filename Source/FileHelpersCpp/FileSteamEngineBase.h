#pragma once
#include <string>
#include <system_error>
#include <vector>
#include "FileEngineBase.h"

namespace file_helpers_cpp
{
	/// <summary>
	/// 表示读取文本行记录的引擎。文件流的读取方式。
	/// </summary>
	class __declspec(dllexport) FileSteamEngineBase : public FileEngineBase
	{
	protected:
		FileSteamEngineBase() = default;

		~FileSteamEngineBase() = default;

	public:
		//int CountLines(const std::string& path, std::error_code error) const override;
		//std::string ReadAllText(const std::string path, std::error_code error) const override;
		//std::vector<std::string> ReadAllLines(const std::string path, std::error_code error) const override;
		//std::vector<std::string> ReadAllLines(const std::string path, std::error_code error, int start_line, int max_records) const override;
		//std::vector<std::vector<std::string>> ReadFileAsStringVector(const std::string path, std::error_code error) const override;
		//std::vector<std::vector<double>> ReadFileAsDoubleVector(const std::string path, std::error_code error) const override;

		/// <summary>
		/// 统计一个文件的行数。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>文件中所有文本的行数。</returns>
		int CountLines(const std::string& path, std::error_code error) const override;

		/// <summary>
		/// 打开一个文本文件，将文件中的所有文本读取到一个字符串中，然后关闭此文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_text">包含文件中所有文本的字符串。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		bool ReadAllText(const std::string& path, std::string& out_all_text, std::error_code error) const override;

		/// <summary>
		/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		bool ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error) const override;

		/// <summary>
		/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
		/// <param name="error">错误信息。</param>
		/// <param name="start_line">开始读取文本的起始行。从1开始。</param>
		/// <param name="max_records">要读取的最大记录数。Int32.MaxValue或-1表示读取所有记录。</param>
		/// <returns>是否完成读取操作。</returns>
		bool ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error, int start_line, int max_records) const override;

		/// <summary>
		/// 创建一个新文件，向其中写入指定的字符串，然后关闭文件。 如果目标文件已存在，则覆盖该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入文件的字符串。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		bool WriteAllText(const std::string& path, const std::string& contents, std::error_code error) const override;

		/// <summary>
		/// 创建一个新文件，向其中写入一个字符串集合，然后关闭该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入到文件中的行。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		bool WriteAllLines(const std::string& path, const std::vector<std::string>& contents, std::error_code error) const override;
	};
}
