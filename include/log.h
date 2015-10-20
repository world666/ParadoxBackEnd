#ifndef LOG
#define LOG

#include <iostream>
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

class Log
{
public:
    static Log& getInstance();
    static Logger& getLogger();
	Log();
	void Initialize();
	void LogWrite(std::string msg);

	std::string FileName;
	int MaxFilesCount;
	long MaxFileSize;

private:

	Logger _logger;

	const std::string PATTERN;
};

std::string ToString(const char* fmt, ...);


#endif


