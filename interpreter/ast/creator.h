#pragma once

void charbuffer_open();
void charbuffer_append(char c);
const char* charbuffer_flush();

const char* interpreter_newstr(const char* str);