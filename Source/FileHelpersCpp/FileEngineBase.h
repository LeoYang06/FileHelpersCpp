#pragma once
#include <string>
#include <system_error>
#include <vector>

namespace file_helpers_cpp
{
	/// <summary>
	/// 表示读取文本行记录的引擎。内存映射文件的方式读取。
	/// </summary>
	class __declspec(dllexport) FileEngineBase
	{
	protected:
		FileEngineBase() = default;
		
		~FileEngineBase() = default;

	public:
		/// <summary>
		/// 判断文件是否存在。
		/// </summary>
		/// <param name="file_name">文件名称。</param>
		/// <returns>是否存在。</returns>
		bool FileExists(const std::string& file_name) const;

		/// <summary>
		/// 按指定大小初始化文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="size">分配的文件大小。</param>
		void AllocateFile(const std::string& path, long size = 0) const;

		/// <summary>
		/// 按指定大小为文件预分配磁盘空间。内存映射文件时建议使用。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="error">错误信息。</param>
		/// <param name="size">预分配的文件大小。</param>
		/// <returns>预分配文件是否成功。</returns>
		bool PreAllocateFile(const std::string& path, std::string& error, long size = 0) const;

		/// <summary>
		/// 统计一个文件的行数。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>文件中所有文本的行数。</returns>
		virtual int CountLines(const std::string& path, std::error_code error) const = 0;

		/// <summary>
		/// 打开一个文本文件，将文件中的所有文本读取到一个字符串中，然后关闭此文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_text">包含文件中所有文本的字符串。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		virtual bool ReadAllText(const std::string& path, std::string& out_all_text, std::error_code error) const = 0;

		/// <summary>
		/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		virtual bool ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error) const = 0;

		/// <summary>
		/// 打开一个文本文件，将文件的所有行读入一个字符串向量，然后关闭该文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_all_lines">包含文件所有行的字符串向量。</param>
		/// <param name="error">错误信息。</param>
		/// <param name="start_line">开始读取文本的起始行。从1开始。</param>
		/// <param name="max_records">要读取的最大记录数。Int32.MaxValue或-1表示读取所有记录。</param>
		/// <returns>是否完成读取操作。</returns>
		virtual bool ReadAllLines(const std::string& path, std::vector<std::string>& out_all_lines, std::error_code error, int start_line, int max_records) const = 0;

		/// <summary>
		/// 打开一个文本文件，将文件中的所有文本读取到一个字符串类型的二维向量，然后关闭此文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_string_vector">包含文件所有行的字符串类型的二维向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		virtual bool ReadFileAsStringVector(const std::string& path, std::vector<std::vector<std::string>>& out_string_vector, std::error_code error) const = 0;

		/// <summary>
		/// 打开一个文本文件，将文件中的所有文本读取到一个字符串类型的二维向量，然后关闭此文件。
		/// </summary>
		/// <param name="path">文件路径。</param>
		/// <param name="out_double_vector">包含文件所有行的double类型的二维向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成读取操作。</returns>
		virtual bool ReadFileAsDoubleVector(const std::string& path, std::vector<std::vector<double>>& out_double_vector, std::error_code error) const = 0;

		/// <summary>
		/// 创建一个新文件，向其中写入指定的字符串，然后关闭文件。 如果目标文件已存在，则覆盖该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入文件的字符串。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		virtual bool WriteAllText(const std::string& path, const std::string& contents, std::error_code error) const = 0;

		/// <summary>
		/// 创建一个新文件，向其中写入一个字符串集合，然后关闭该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入到文件中的行。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		virtual bool WriteAllLines(const std::string& path, const std::vector<std::string>& contents, std::error_code error) const = 0;

		/// <summary>
		/// 创建一个新文件，向其中写入一个字符串类型的二维向量的集合，然后关闭该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入文件的字符串类型的二维向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		virtual bool WriteAllStringVector(const std::string& path, const std::vector<std::vector<std::string>>& contents, std::error_code error) const = 0;

		/// <summary>
		/// 创建一个新文件，向其中写入一个double类型的二维向量的集合，然后关闭该文件。
		/// </summary>
		/// <param name="path">要写入的文件。</param>
		/// <param name="contents">要写入文件的double类型的二维向量。</param>
		/// <param name="error">错误信息。</param>
		/// <returns>是否完成写入操作。</returns>
		virtual bool WriteAllDoubleVector(const std::string& path, const std::vector<std::vector<double>>& contents, std::error_code error) const = 0;
	};
}
