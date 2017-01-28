#define DEBUG_LOG

#ifdef DEBUG_LOG
#include <iostream>
#define LOG(toLog) std::cout << toLog << std::endl
#else
#define LOG(toLog)
#endif
