#include <stdint.h>
#include <stdio.h>
#include "infrastructure.h"

#if defined(DEBUG_APP)
void appAssertDebug(uint32_t val, const char* file, int line)
{
  while (1) {
//    printf("F: %s, L: %u; E: %u\n", file, line, (unsigned int)(val));
  }
}
#endif // DEBUG_APP

#if defined(LOG_APP)
void appLog(char* msg)
{
//  printf("%s", msg);
}
#endif // LOG_APP
