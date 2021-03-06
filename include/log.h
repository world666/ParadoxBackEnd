#include <iostream>

#if not defined LOG && defined _LOG
#define LOG

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
#if not defined LOG && not defined _LOG
std::string ToString(const char* fmt, ...);
#define LOG4CPLUS_DEBUG(ignore, text) printf(text); printf("\n");
#endif




