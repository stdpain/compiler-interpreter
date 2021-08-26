#pragma once
#include <iostream>

#define USE_PAIN_LOG 1

#ifdef USE_PAIN_LOG
#define PAIN_LOG(var)                                                                            \
    std::cout << __FILE__ << "+" << __LINE__ << ":" << __FUNCTION__ << " " << #var << ":" << var \
              << std::endl;
#else
#define PAIN_LOG(var)
#endif
