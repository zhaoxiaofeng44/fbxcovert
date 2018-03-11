
#pragma once

#include <iostream>
#include <assert.h>
#include <string>
#include <iomanip>
#include <memory>

#define LOG						std::cout
#define ENDL					std::endl
#define ENDN					"\n"

#define LOG_ERROR(x)				Log::log_error(x)
#define LOG_ASSERT(x)				Log::log_error(x)
#define LOG_SHADER_ERROR(x,xx)		Log::log_shader_error(x,xx)
#define STR_FORMAT(x, ...)			Log::str_format(x, ##__VA_ARGS__)
//#define FOO(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define SAFE_DELETE(x) if( x ) { delete (x); (x) = NULL; }

namespace Log
{
	inline void log_error(const std::string &msg)
	{
		//MessageBox(NULL, msg.c_str(), "Error", MB_OK);
	}

	inline void log_assert(const std::string &msg)
	{
		//MessageBox(NULL, msg.c_str(), "Error", MB_OK);
		//assert(0 && msg.c_str());
	}

	inline void log_shader_error(const std::string &path, const std::string &msg)
	{
		std::string err = path;
		err.append(" : ").append(msg);
		LOG << err.c_str() << ENDL;
		//MessageBox(NULL, err.c_str(), "Error", MB_OK);
		//assert(0 && msg.c_str());
	}

	template<typename ... Args>
	std::string str_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}




}

