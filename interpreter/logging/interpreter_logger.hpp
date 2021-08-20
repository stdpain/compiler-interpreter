#pragma once
#include <iostream>

#define PAIN_LOG(var)                                                                     \
    std::cout << __FILE__ <<"+" << __LINE__ << ":" << __FUNCTION__<< " "  << #var << ":" << var \
              << std::endl;
