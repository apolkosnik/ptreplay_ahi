/* Automatically generated header! Do not edit! */

#ifndef _PPCINLINE_PTREPLAY_H
#define _PPCINLINE_PTREPLAY_H

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif /* !__PPCINLINE_MACROS_H */

#ifndef PTREPLAY_BASE_NAME
#define PTREPLAY_BASE_NAME PTReplayBase
#endif /* !PTREPLAY_BASE_NAME */

#define PTSongPattern(__p0, __p1) \
	LP2(90, UBYTE , PTSongPattern, \
		APTR , __p0, a0, \
		UWORD , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTGetChan() \
	LP0(162, BYTE , PTGetChan, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSetPos(__p0, __p1) \
	LP2NR(144, PTSetPos, \
		APTR , __p0, a0, \
		UBYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTOnChannel(__p0, __p1) \
	LP2NR(132, PTOnChannel, \
		APTR , __p0, a0, \
		BYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTResume(__p0) \
	LP1(60, ULONG , PTResume, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTGetPri() \
	LP0(156, BYTE , PTGetPri, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTStartFade(__p0, __p1) \
	LP2NR(126, PTStartFade, \
		APTR , __p0, a0, \
		UBYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTPlay(__p0) \
	LP1(42, ULONG , PTPlay, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTGetSample(__p0, __p1) \
	LP2(168, struct PTSample *, PTGetSample, \
		APTR , __p0, a0, \
		WORD , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTFreeMod(__p0) \
	LP1NR(120, PTFreeMod, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSetVolume(__p0, __p1) \
	LP2NR(72, PTSetVolume, \
		APTR , __p0, a0, \
		UBYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTPatternData(__p0, __p1, __p2) \
	LP3(102, APTR , PTPatternData, \
		APTR , __p0, a0, \
		UBYTE , __p1, d0, \
		UBYTE , __p2, d1, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSetupMod(__p0) \
	LP1(114, APTR , PTSetupMod, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSongLen(__p0) \
	LP1(84, UBYTE , PTSongLen, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTPatternPos(__p0) \
	LP1(96, UBYTE , PTPatternPos, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTLoadModule(__p0) \
	LP1(30, APTR , PTLoadModule, \
		CONST_STRPTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTPause(__p0) \
	LP1(54, ULONG , PTPause, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSetPri(__p0) \
	LP1NR(150, PTSetPri, \
		BYTE , __p0, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTInstallBits(__p0, __p1, __p2, __p3, __p4) \
	LP5NR(108, PTInstallBits, \
		APTR , __p0, a0, \
		BYTE , __p1, d0, \
		BYTE , __p2, d1, \
		BYTE , __p3, d2, \
		BYTE , __p4, d3, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTFade(__p0, __p1) \
	LP2NR(66, PTFade, \
		APTR , __p0, a0, \
		UBYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTStop(__p0) \
	LP1(48, ULONG , PTStop, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTOffChannel(__p0, __p1) \
	LP2NR(138, PTOffChannel, \
		APTR , __p0, a0, \
		BYTE , __p1, d0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTUnloadModule(__p0) \
	LP1NR(36, PTUnloadModule, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#define PTSongPos(__p0) \
	LP1(78, UBYTE , PTSongPos, \
		APTR , __p0, a0, \
		, PTREPLAY_BASE_NAME, 0, 0, 0, 0, 0, 0)

#endif /* !_PPCINLINE_PTREPLAY_H */
