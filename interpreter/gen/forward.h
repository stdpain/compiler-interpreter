#pragma once

struct MSParseContext;
// forward variables for flex/yacc
struct yy_buffer_state;
int mslex_init_extra(MSParseContext* user_defined, void** scanner);
yy_buffer_state* ms_scan_bytes(const char* bytes, int len, void* yyscanner);
int msparse(MSParseContext* ctx);
void mslex_destroy(void*scanner);