#ifndef APP_H
#define APP_H

#include "native_gecko.h"

void appInit (void);
void appHandleEvents(struct gecko_cmd_packet *evt);
void appServiceStart(void);
void appServiceStop(void);

#endif /* APP_H */
