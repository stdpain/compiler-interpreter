//string_pool.h

#ifndef STRING_POOL_H
#define STRING_POOL_H
typedef struct interpreter_tag Interpreter;
typedef struct MString_tag {
    char* str;
    int is_src;
    int ref_count;
} MString;

MString* allocMString(char* ch, int is_src);
MString* MStringRetain(MString* mstr);
MString* MStringRelease(MString* mstr);

#endif