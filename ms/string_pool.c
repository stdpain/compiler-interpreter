#include "../include/string_pool.h"
#include "../include/interpreter.h"
#include "../include/MEM.h"

MString *allocMString(char *ch, int is_src)
{
     // MString *mstring = Interpreter_malloc(sizeof(MString));
     MString *mstring = MEM_malloc(sizeof(MString));
     mstring->is_src = is_src;
     mstring->ref_count = 1;
     mstring->str = ch;
     return mstring;
}
MString *MStringRetain(MString *mstr)
{
     if (mstr->is_src)
          return mstr;
     else
     {
          mstr->ref_count++;
          printf("retain:%s refcount:%d\n", mstr->str, mstr->ref_count);
     }
     return mstr;
}
MString *MStringRelease(MString *mstr)
{
     if (mstr->is_src)
          return mstr;
     else
     {
          mstr->ref_count--;
          printf("release:%s refcount:%d\n", mstr->str, mstr->ref_count);
          if (mstr->ref_count == 0)
          {
               MEM_free(mstr->str);
               MEM_free(mstr);
               return NULL;
          }
     }
     return mstr;
}