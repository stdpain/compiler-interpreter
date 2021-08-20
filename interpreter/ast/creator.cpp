#include "creator.h"
#include <memory>
#include <cstring>
#include <string>

static std::string mbuffer;

void charbuffer_open() {
    mbuffer.reserve(256);
    mbuffer.clear();
}

void charbuffer_append(char c) {
    mbuffer.push_back(c);
}

const char* charbuffer_flush() {
    return mbuffer.c_str();
}
