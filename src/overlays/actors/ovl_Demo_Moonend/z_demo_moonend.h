#ifndef Z_DEMO_MOONEND_H
#define Z_DEMO_MOONEND_H

#include "global.h"

struct DemoMoonend;

typedef struct DemoMoonend {
    /* 0x000 */ Actor actor;
    /* 0x144 */ char unk_144[0x1A0];
} DemoMoonend; // size = 0x2E4

extern const ActorInit Demo_Moonend_InitVars;

#endif // Z_DEMO_MOONEND_H
