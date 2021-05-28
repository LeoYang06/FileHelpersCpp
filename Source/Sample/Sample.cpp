// sample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <chrono>
#include <iostream>
#include <ppltasks.h>
#include "../FileHelpersCpp/DelimitedFileMMFEngine.h"
#include "../FileHelpersCpp/FileMMFEngineBase.h"
#include "../FileHelpersCpp/DelimitedFileSteamEngine.h"
#include "../FileHelpersCpp/StringConverter.h"

using namespace std;
using namespace file_helpers_cpp;

struct DateTime
{
	int dt_msec;  /*毫秒，正常范围0-999*/
	int dt_sec;  /*秒，正常范围0-59， 但允许至61*/
	int dt_min;  /*分钟，0-59*/
	int dt_hour; /*小时， 0-23*/
	int dt_mday; /*日，即一个月中的第几天，1-31*/
	int dt_mon;  /*月， 从一月算起，0-11*/
	int dt_year;  /*年， 从1900至今已经多少年*/
	int dt_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
	int dt_yday; /*从今年1月1日到目前的天数，范围0-365*/
	int dt_isdst; /*日光节约时间的旗标*/
};


inline DateTime get_time_interval(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end)
{
	const auto milliseconds_duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(milliseconds_duration);
	const auto milliseconds = milliseconds_duration - seconds;

	DateTime dt;
	dt.dt_sec = seconds.count();
	dt.dt_msec = milliseconds.count();
	return dt;
}


inline auto ReadWriteAllLines(const std::string& readPath, const std::string& writePath, const DelimitedFileMmfEngine& dfm_engine)
{
	return [readPath, writePath, dfm_engine]
	{
		std::cout << "正在读取或写入文件..." << std::endl;
		const auto r_start = std::chrono::system_clock::now();

		const std::error_code error;
		std::vector<std::string> read_all_lines;
		bool isSuf = dfm_engine.ReadAllLines(readPath, read_all_lines, error);
		const auto record_count = read_all_lines.size();

		const auto r_end = std::chrono::system_clock::now();
		const auto r_dt = get_time_interval(r_start, r_end);

		const auto w_start = std::chrono::system_clock::now();

		const bool result = dfm_engine.WriteAllLines(writePath, read_all_lines, error);
		if (!result)
		{
			std::string msg = error.message();
		}
		read_all_lines.clear();
		read_all_lines.shrink_to_fit();

		const auto w_end = std::chrono::system_clock::now();
		const auto w_dt = get_time_interval(w_start, w_end);

		const std::string output_str = StringFormat("读取点云耗时：%u.%us   点数量：%u   写入点云耗时：%u.%us", r_dt.dt_sec, r_dt.dt_msec, record_count, w_dt.dt_sec, w_dt.dt_msec);
		std::cout << output_str << std::endl;
	};
}

inline auto ReadWriteStringVector(const std::string& readPath, const std::string& writePath, const DelimitedFileMmfEngine& dfm_engine)
{
	return[readPath, writePath, dfm_engine]
	{
		std::cout << "正在读取或写入文件..." << std::endl;
		const auto r_start = std::chrono::system_clock::now();

		const std::error_code error;
		std::vector<std::vector<std::string>> read_vectors;
		dfm_engine.ReadFileAsStringVector(readPath, read_vectors, error);
		const auto record_count = read_vectors.size();

		const auto r_end = std::chrono::system_clock::now();
		const auto r_dt = get_time_interval(r_start, r_end);

		const auto w_start = std::chrono::system_clock::now();

		const bool result = dfm_engine.WriteAllStringVector(writePath, read_vectors, error);
		if (!result)
		{
			std::string msg = error.message();
		}

		const auto w_end = std::chrono::system_clock::now();
		const auto w_dt = get_time_interval(w_start, w_end);

		const std::string output_str = StringFormat("读取点云耗时：%u.%us   点数量：%u   写入点云耗时：%u.%us", r_dt.dt_sec, r_dt.dt_msec, record_count, w_dt.dt_sec, w_dt.dt_msec);
		std::cout << output_str << std::endl;
	};
}

int main()
{
	//const std::string readPath = "D:\\FileHelpersCpp测试数据\\test.xyz";
	const std::string readPath = "D:\\FileHelpersCpp测试数据\\test.xyz";
	const std::string writePath = "D:\\FileHelpersCpp测试数据\\test_write.xyz";

	DelimitedFileMmfEngine dfm_engine(" ");
	DelimitedFileSteamEngine dfs_engine(" ");

	const auto t1 = concurrency::create_task(ReadWriteStringVector(readPath, writePath, dfm_engine));
	t1.then(ReadWriteStringVector(readPath, writePath, dfm_engine))
	.then(ReadWriteStringVector(readPath, writePath, dfm_engine))
	.then(ReadWriteStringVector(readPath, writePath, dfm_engine))
	.then(ReadWriteStringVector(readPath, writePath, dfm_engine));

	//const auto t21 = concurrency::create_task(ReadWriteAllLines(readPath, writePath, dfm_engine));
	//t21.then(ReadWriteAllLines(readPath, writePath, dfm_engine))
	//	.then(ReadWriteAllLines(readPath, writePath, dfm_engine))
	//	.then(ReadWriteAllLines(readPath, writePath, dfm_engine))
	//	.then(ReadWriteAllLines(readPath, writePath, dfm_engine));

	std::cin.clear();
	std::cin.ignore();
	return 0;

}

