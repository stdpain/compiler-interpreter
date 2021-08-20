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

const char* interpreter_newstr(const char* str) {
    size_t sz = strlen(str);
    char* mem = static_cast<char*>(malloc(sz + 1));
    memcpy(mem, str, sz + 1);
    return mem;
}