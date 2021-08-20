#include <stdio.h>

#define PAIN_LOG(fmt, args...) printf("%s:%d" fmt "\n", __FUNCTION__, __LINE__, ##args);
