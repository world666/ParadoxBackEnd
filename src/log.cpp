#include "log.h"

Log& Log::getInstance()
{
	static Log instance; // Guaranteed to be destroyed.
	return instance;	 // Instantiated on first use.
}

Logger& Log::getLogger()
{
	return getInstance()._logger;
}

Log::Log() : PATTERN("%d{%m/%d/%y  %H:%M:%S}  - %m [%l]%n")
{
	FileName = "Test.log";
	MaxFilesCount = 5;
	MaxFileSize = 500000;
}

void Log::Initialize()
{
	//SharedObjectPtr<Appender> append (new ConsoleAppender());
	SharedAppenderPtr  append(new  RollingFileAppender(FileName, MaxFileSize, MaxFilesCount));
	append->setName("append for  test");

	/* Instantiate a layout object */
	std::auto_ptr<Layout> layout(new PatternLayout(PATTERN));

	/* Attach the layout object to the appender */
	append->setLayout( layout );

	/* Instantiate a logger object */
	_logger = Logger::getInstance("test");

	/* Attach the appender object to the  logger  */
	_logger.addAppender(append);

	/* Set a priority for the logger  */
	_logger.setLogLevel(ALL_LOG_LEVEL);
}

