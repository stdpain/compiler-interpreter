//string_pool.h
typedef struct MString_tag
{
    char *str;
    int is_src;
    int ref_count;
} MString;


