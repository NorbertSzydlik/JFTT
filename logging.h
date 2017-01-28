#define DEBUG_LOG

#ifdef DEBUG_LOG
#include <iostream>
#define LOG(toLog) std::cout << "log" << std::endl; std::cout << toLog << std::endl
#else
#define LOG(toLog)
#endif
