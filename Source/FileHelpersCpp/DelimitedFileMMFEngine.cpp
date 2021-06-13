#include "pch.h"
#include <map>
#include <queue>
#include "mio.hpp"
#include "DelimitedFileMMFEngine.h"
#include "StringUtils.h"

using namespace file_helpers_cpp;

/// <summary>
/// 有参构造函数。
/// </summary>
/// <param name="delimiter"></param>
DelimitedFileMmfEngine::DelimitedFileMmfEngine(const std::string& delimiter)
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
bool DelimitedFileMmfEngine::ReadFileAsStringVector(const std::string& path, std::vector<std::vector<std::string>>& out_string_vector, std::error_code error) const
{
	const int line_count = FileMmfEngineBase::CountLines(path, error);
	out_string_vector.reserve(line_count);

	mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	if (error)
	{
		return false;
	}

	std::string str_line;

	for (auto& c : read_mmap)
	{
		if (c == '\r')
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
			continue;
		}
		if (c == '\n')
		{
			continue;
		}
		str_line += c;
	}

	read_mmap.unmap();
	return true;
}

/// <summary>
/// 打开一个文本文件，将文件中的所有文本读取到一个字符串类型的二维向量，然后关闭此文件。
/// </summary>
/// <param name="path">文件路径。</param>
/// <param name="out_double_vector">包含文件所有行的double类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成读取操作。</returns>
bool DelimitedFileMmfEngine::ReadFileAsDoubleVector(const std::string& path, std::vector<std::vector<double>>& out_double_vector, std::error_code error) const
{
	const int line_count = FileMmfEngineBase::CountLines(path, error);
	out_double_vector.reserve(line_count);

	mio::mmap_source read_mmap = mio::make_mmap_source(path, error);
	if (error)
	{
		return false;
	}

	std::string str_line;

	for (auto& c : read_mmap)
	{
		if (c == '\r')
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
			continue;
		}
		if (c == '\n')
		{
			continue;
		}
		str_line += c;
	}

	read_mmap.unmap();
	return true;
}

/// <summary>
/// 创建一个新文件，向其中写入一个字符串类型的二维向量的集合，然后关闭该文件。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的字符串类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool DelimitedFileMmfEngine::WriteAllStringVector(const std::string& path, const std::vector<std::vector<std::string>>& contents, std::error_code error) const
{
	try
	{
		long characters_size = 0;
		const int delimiter_size = delimiter.size();

		// 计算contents占用的字符串表现形式的最大长度。
		for (std::vector<std::string> line_vector : contents)
		{
			const int rn_size = 1;
			int field_index = 0;
			for (auto field : line_vector)
			{
				field_index++;
				characters_size += field.size();
				if (field_index < line_vector.size())
					characters_size += delimiter_size;
			}
			// 加上换行符长度
			characters_size += 1;
		}

		// 始终覆盖创建新文件。
		std::string error_msg;
		const bool result = PreAllocateFile(path, error_msg, characters_size);
		if (!result)
		{
			return false;
		}

		mio::mmap_sink rw_mmap = mio::make_mmap_sink(path, error);
		if (error)
		{
			return false;
		}

		int char_index = 0;
		int line_index = 0;
		for (std::vector<std::string> line_vector : contents)
		{
			line_index++;
			int field_index = 0;
			for (auto field : line_vector)
			{
				field_index++;
				for (auto c : field)
				{
					rw_mmap[char_index] = c;
					char_index++;
				}

				// 如果还没到每行的最后一个字段，则需要写入分隔符。
				if (field_index < line_vector.size())
				{
					for (auto c : delimiter)
					{
						rw_mmap[char_index] = c;
						char_index++;
					}
				}
			}
			rw_mmap[char_index] = '\n';
			char_index++;

			// 注意：经测试，每80w行左右同步一次性能和内存占用最佳。
			if (line_index >= 800000)
			{
				rw_mmap.sync(error);
				line_index = 0;
			}
		}

		rw_mmap.sync(error);
		rw_mmap.unmap();
		return true;
	}
	catch (std::exception& ex)
	{
		auto msg = ex.what();
		return false;
	}
}

/// <summary>
/// 创建一个新文件，向其中写入一个double类型的二维向量的集合，然后关闭该文件。文件存在则覆盖。
/// </summary>
/// <param name="path">要写入的文件。</param>
/// <param name="contents">要写入文件的double类型的二维向量。</param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成写入操作。</returns>
bool DelimitedFileMmfEngine::WriteAllDoubleVector(const std::string& path, const std::vector<std::vector<double>>& contents, std::error_code error) const
{
	try
	{
		long characters_size = 0;
		const int delimiter_size = delimiter.size();

		// 计算contents占用的字符串表现形式的最大长度。
		for (std::vector<double> line_vector : contents)
		{
			const int rn_size = 1;
			int field_index = 0;
			for (auto field : line_vector)
			{
				field_index++;
				const auto field_str = std::to_string(field);
				characters_size += field_str.size();
				if (field_index < line_vector.size())
					characters_size += delimiter_size;
			}
			// 加上换行符长度
			characters_size += 1;
		}

		// 始终覆盖创建新文件。
		std::string error_msg;
		const bool result = PreAllocateFile(path, error_msg, characters_size);
		if (!result)
		{
			return false;
		}

		mio::mmap_sink rw_mmap = mio::make_mmap_sink(path, error);
		if (error)
		{
			return false;
		}

		int char_index = 0;
		int line_index = 0;
		for (std::vector<double> line_vector : contents)
		{
			line_index++;
			int field_index = 0;
			for (auto field : line_vector)
			{
				field_index++;
				const auto field_str = std::to_string(field);
				for (auto c : field_str)
				{
					rw_mmap[char_index] = c;
					char_index++;
				}

				if (field_index < line_vector.size())
				{
					for (auto c : delimiter)
					{
						rw_mmap[char_index] = c;
						char_index++;
					}
				}
			}

			rw_mmap[char_index] = '\n';
			char_index++;

			// 注意：经测试，每80w行左右同步一次性能和内存占用最佳。
			if (line_index >= 800000)
			{
				rw_mmap.sync(error);
				line_index = 0;
			}
		}

		rw_mmap.sync(error);
		rw_mmap.unmap();
		return true;
	}
	catch (std::exception& ex)
	{
		auto msg = ex.what();
		return false;
	}
}

/// <summary>
/// 打开指定文件，批量修改指定行列字段的值，然后关闭该文件。新值和旧值长度一致，否则报内存映射文件异常。
/// </summary>
/// <param name="path">要修改的文件。</param>
/// <param name="contents">要修改文件的字符串类型的二维数据对。<行索引（从0开始），<字段索引（从0开始，以分隔符分割），新的字段值>></param>
/// <param name="error">错误信息。</param>
/// <returns>是否完成修改操作。</returns>
bool DelimitedFileMmfEngine::BatchModifyFieldValues(const std::string& path, const std::map<int, std::map<int, std::string>>& contents, std::error_code error) const
{
	try
	{
		mio::mmap_sink rw_mmap = mio::make_mmap_sink(path, error);
		if (error)
		{
			return false;
		}

		// 读取的字符索引。
		int char_index = 0;
		// 读取的行索引。
		int line_index = 0;
		// 读取的行文本。
		std::string str_line;

		// 新行开始的首字符索引。
		int new_line_char_index = 0;

		// 表示上一行的待修改字段的索引记录。<第几个字段,字段开始的首字符索引>
		std::map<int, int> last_line_field_index_record;

		for (auto c : rw_mmap)
		{
			if (contents.empty())
			{
				return true;
			}
			if (c == '\r')
			{
				char_index++;
				continue;
			}
			if (c == '\n')
			{
				// 空行判断
				if (std::strlen(str_line.c_str()) == 0)
				{
					char_index++;
					// 一行读取完毕后，累计读到的字符索引则为新行索引。这里索引已经 +1。
					new_line_char_index = char_index;
					continue;
				}

				// 从待修改行中查找需要修改的行。
				auto iter = contents.find(line_index);
				// 找到待修改行。
				if (iter != contents.end())
				{
					std::vector<std::string> str_fields = Split(str_line, this->delimiter, true);

					int field_index = 0;
					// 需要修改的行存在则提前将该行各字段的char索引存储下来，供修改使用。
					for (auto field : str_fields)
					{
						if (last_line_field_index_record.empty())
						{
							last_line_field_index_record.insert(std::pair<int, int>(field_index, new_line_char_index));
						}
						else
						{
							// 从第二个字段开始，需要累加前一个字段的size和分隔符的size。
							const auto end_iter = last_line_field_index_record.rbegin();
							const int last_field_index = end_iter->second;
							const int last_field_size = static_cast<int>(str_fields[end_iter->first].size());
							const int delimiter_size = static_cast<int>(delimiter.size());
							const int field_start_index = last_field_index + last_field_size + delimiter_size;
							last_line_field_index_record.insert(std::pair<int, int>(field_index, field_start_index));
						}
						field_index++;
					}

					// 根据对应字段的索引开始逐一字符修改。
					std::map<int, std::string> modified_fields = iter->second;
					for (const auto& modified_field : modified_fields)
					{
						int modify_index = last_line_field_index_record[modified_field.first];
						for (auto cha : modified_field.second)
						{
							rw_mmap[modify_index] = cha;
							modify_index++;
						}
					}
				}
				last_line_field_index_record.clear();
				str_line.clear();

				char_index++;
				line_index++;
				// 一行读取完毕后，累计读到的字符索引则为新行索引。这里索引已经 +1。
				new_line_char_index = char_index;
				continue;
			}
			char_index++;
			str_line += c;
		}

		rw_mmap.sync(error);
		rw_mmap.unmap();
		return true;
	}
	catch (std::exception& ex)
	{
		auto msg = ex.what();
		return false;
	}
}
