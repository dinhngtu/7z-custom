#ifndef __MITIGATIONS_H
#define __MITIGATIONS_H

#include "7zTypes.h"

EXTERN_C_BEGIN

#ifdef _WIN32

BOOL EnableMitigations(void);
BOOL EnableChildProcessMitigation(void);

#else

static inline BOOL EnableMitigations(void) { return TRUE; }
static inline BOOL EnableChildProcessMitigation(void) { return TRUE; }

#endif

EXTERN_C_END

#endif
