#ifndef _VBCCINLINE_PTPLAY_H
#define _VBCCINLINE_PTPLAY_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

APTR __PtInit(__reg("a6") struct Library *, __reg("a1") UBYTE * buf, __reg("d0") LONG bufsize, __reg("d1") LONG frequency, __reg("d2") ULONG modtype)="\tjsr\t-30(a6)";
#define PtInit(buf, bufsize, frequency, modtype) __PtInit(PtPlayBase, (buf), (bufsize), (frequency), (modtype))

VOID __PtRender(__reg("a6") struct Library *, __reg("a0") APTR mod, __reg("a1") BYTE * destbuf1, __reg("a2") BYTE * destbuf2, __reg("d0") LONG bufmodulo, __reg("d1") LONG numsp, __reg("d2") LONG scale, __reg("d3") LONG depth, __reg("d4") LONG channels)="\tjsr\t-36(a6)";
#define PtRender(mod, destbuf1, destbuf2, bufmodulo, numsp, scale, depth, channels) __PtRender(PtPlayBase, (mod), (destbuf1), (destbuf2), (bufmodulo), (numsp), (scale), (depth), (channels))

ULONG __PtTest(__reg("a6") struct Library *, __reg("a0") CONST_STRPTR filename, __reg("a1") UBYTE * buf, __reg("d0") LONG bufsize)="\tjsr\t-42(a6)";
#define PtTest(filename, buf, bufsize) __PtTest(PtPlayBase, (filename), (buf), (bufsize))

VOID __PtCleanup(__reg("a6") struct Library *, __reg("a0") APTR mod)="\tjsr\t-48(a6)";
#define PtCleanup(mod) __PtCleanup(PtPlayBase, (mod))

VOID __PtSetAttrs(__reg("a6") struct Library *, __reg("a0") APTR mod, __reg("a1") struct TagItem * taglist)="\tjsr\t-54(a6)";
#define PtSetAttrs(mod, taglist) __PtSetAttrs(PtPlayBase, (mod), (taglist))

ULONG __PtGetAttr(__reg("a6") struct Library *, __reg("a0") APTR mod, __reg("d0") ULONG tagitem, __reg("a1") ULONG * storagePtr)="\tjsr\t-60(a6)";
#define PtGetAttr(mod, tagitem, storagePtr) __PtGetAttr(PtPlayBase, (mod), (tagitem), (storagePtr))

ULONG __PtSeek(__reg("a6") struct Library *, __reg("a0") APTR mod, __reg("d0") ULONG time)="\tjsr\t-66(a6)";
#define PtSeek(mod, time) __PtSeek(PtPlayBase, (mod), (time))

#endif /*  _VBCCINLINE_PTPLAY_H  */
