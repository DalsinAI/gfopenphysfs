#ifndef _PROTO_PHYSFS_H
#define _PROTO_PHYSFS_H
#include <exec/types.h>
/* C linkage, as <inline/physfs.h> (inside <physfs.h>'s extern "C") declares it */
#ifdef __cplusplus
extern "C" {
#endif
extern struct Library *PhysFSBase;
#ifdef __cplusplus
}
#endif
#define PHYSFSNAME "physfs.library"
#include <physfs.h>
#endif
