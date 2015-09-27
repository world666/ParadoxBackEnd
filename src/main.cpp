#include <iostream>

#include "log.h"

using namespace std;


int main() 
{ 
	Log& log = Log::getInstance();
	log.FileName = "Paradox.log";
	log.MaxFileSize = 300000;
	log.MaxFilesCount = 2;
	log.Initialize();
	LOG4CPLUS_DEBUG(Log::getLogger(), "Paradox back-end start up");

    return 0;
}
