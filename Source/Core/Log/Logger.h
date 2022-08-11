#pragma once
#include <Windows.h>
#include <iostream>

#include "Core/Base/Singleton.h"

namespace SDBX
{
	class Logger : public Singleton<Logger>
	{
	public:
		enum class LogLevel
		{
			INFO_LOG
			, WARNING_LOG
			, ERROR_LOG
		};

		~Logger() override = default;
		Logger(const Logger& other) = delete;
		Logger(Logger&& other) noexcept = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) noexcept = delete;

		template<typename MESSAGE_TYPE>
		static void Log(LogLevel level, const MESSAGE_TYPE& message);
		template<typename MESSAGE_TYPE>
		static void LogW(LogLevel level, const MESSAGE_TYPE& message);

	private:
		friend class Singleton<Logger>;
		explicit Logger() = default;
	};
	
	template<typename MessageType>
	void Logger::Log(LogLevel level, const MessageType& message)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		std::string strHeader{};
		
		switch (level)
		{
		case LogLevel::INFO_LOG:
			strHeader = "[INFO] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case LogLevel::WARNING_LOG:
			strHeader = "[WARNING] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case LogLevel::ERROR_LOG:
			strHeader = "[ERROR] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			break;
		}
		std::cout << strHeader << message << std::endl;

		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		if (level == LogLevel::ERROR_LOG)
			__debugbreak();
	}

	template<typename MessageType>
	void Logger::LogW(LogLevel level, const MessageType& message)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		std::wstring strHeader{};

		switch (level)
		{
		case LogLevel::INFO_LOG:
			strHeader = L"[INFO] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case LogLevel::WARNING_LOG:
			strHeader = L"[WARNING] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case LogLevel::ERROR_LOG:
			strHeader = L"[ERROR] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			break;
		}
		std::wcout << strHeader << message << std::endl;

		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		if (level == LogLevel::ERROR_LOG)
			__debugbreak();
	}
}

#define STR(x) #x
#define STR1(x) STR(x)

#define WSTR(x) L ## #x
#define WSTR1(x) WSTR(x)

#define SDBX_STATIC_ASSERT(exp, msg) static_assert(exp, msg)

#if defined(SDBX_LOGGER_RELEASE_ASSERT) || defined(_DEBUG) || defined(DEBUG)
	#define SDBX_ASSERT(exp, msg) if (!(exp)) SDBX::Logger::Log<std::string>(SDBX::Logger::LogLevel::ERROR_LOG, std::string(#exp) + ": " + msg);
	#define SDBX_ASSERT_AS_WARNING(exp, msg) if (!(exp)) SDBX::Logger::Log<std::string>(SDBX::Logger::LogLevel::WARNING_LOG, std::string(#exp) + ": " + msg);
	#define SDBX_LOG(logLevel, msg) SDBX::Logger::Log<std::string>(SDBX::Logger::LogLevel::##logLevel, msg);

	#define SDBX_W_ASSERT(exp, msg) if (!(exp)) SDBX::Logger::LogW<std::wstring>(SDBX::Logger::LogLevel::ERROR_LOG, std::wstring(WSTR1(exp)) + L": " + msg);
	#define SDBX_W_ASSERT_AS_WARNING(exp, msg) if (!(exp)) SDBX::Logger::LogW<std::wstring>(SDBX::Logger::LogLevel::WARNING_LOG, std::wstring(WSTR1(exp)) + L": " + msg);
	#define SDBX_W_LOG(logLevel, msg) SDBX::Logger::LogW<std::wstring>(SDBX::Logger::LogLevel::##logLevel, msg);
#else
	#define SDBX_ASSERT(exp, msg)
	#define SDBX_ASSERT_AS_WARNING(exp, msg)
	#define SDBX_LOG(logLevel, msg)

	#define SDBX_W_ASSERT(exp, msg)
	#define SDBX_W_ASSERT_AS_WARNING(exp, msg)
	#define SDBX_W_LOG(logLevel, msg)
#endif
