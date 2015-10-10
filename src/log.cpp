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
	SharedObjectPtr<Appender> appendConsole (new ConsoleAppender());
	SharedAppenderPtr  appendFile(new  RollingFileAppender(FileName, MaxFileSize, MaxFilesCount));
	appendFile->setName("Paradox File");
	appendConsole->setName("Paradox Console");

	/* Instantiate a layout object */
	std::auto_ptr<Layout> layoutConsole(new PatternLayout(PATTERN));
	std::auto_ptr<Layout> layoutFile(new PatternLayout(PATTERN));

	/* Attach the layout object to the appender */
	appendFile->setLayout( layoutFile );
	appendConsole->setLayout( layoutConsole );

	/* Instantiate a logger object */
	_logger = Logger::getInstance("test");

	/* Attach the appender object to the  logger  */
	_logger.addAppender(appendFile);
	_logger.addAppender(appendConsole);

	/* Set a priority for the logger  */
	_logger.setLogLevel(ALL_LOG_LEVEL);
}

