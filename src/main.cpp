#include <iostream>

#include "log.h"

using namespace std;


int main() 
{ 
	LOG4CPLUS_DEBUG(Log::getLogger(), "Paradox back-end start up");

    return 0;
}
