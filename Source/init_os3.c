
#include "ptreplay_private.h"
#include "ptreplay.library_rev.h"

CONST TEXT __TEXTSEGMENT__ verstag[] = VERSTAG;
CONST TEXT vstring[] = VSTRING;
CONST TEXT libname[] = "ptreplay.library";

#if defined(__MORPHOS__)
CONST ULONG __abox__	= 1;
#endif

struct Module *_PTReplay_PTLoadModule(struct PTReplayBase *, STRPTR name);
VOID _PTReplay_PTUnloadModule(struct PTReplayBase *, struct Module * module);
ULONG _PTReplay_PTPlay(struct PTReplayBase *, struct Module * module);
ULONG _PTReplay_PTStop(struct PTReplayBase *, struct Module * module);
ULONG _PTReplay_PTPause(struct PTReplayBase *, struct Module * module);
ULONG _PTReplay_PTResume(struct PTReplayBase *, struct Module * module);
VOID _PTReplay_PTFade(struct PTReplayBase *, struct Module * module, UBYTE speed);
VOID _PTReplay_PTSetVolume(struct PTReplayBase *, struct Module * module, UBYTE vol);
UBYTE _PTReplay_PTSongPos(struct PTReplayBase *, struct Module * module);
UBYTE _PTReplay_PTSongLen(struct PTReplayBase *, struct Module * module);
UBYTE _PTReplay_PTSongPattern(struct PTReplayBase *, struct Module * module, UWORD Pos);
UBYTE _PTReplay_PTPatternPos(struct PTReplayBase *, struct Module * Module);
APTR _PTReplay_PTPatternData(struct PTReplayBase *, struct Module * Module, UBYTE Pattern, UBYTE Row);
void _PTReplay_PTInstallBits(struct PTReplayBase *, struct Module * Module, BYTE Restart, BYTE NextPattern, BYTE NextRow, BYTE Fade);
struct Module *_PTReplay_PTSetupMod(struct PTReplayBase *, APTR ModuleFile);
void _PTReplay_PTFreeMod(struct PTReplayBase *, struct Module * Module);
void _PTReplay_PTStartFade(struct PTReplayBase *, struct Module * Module, UBYTE speed);
void _PTReplay_PTOnChannel(struct PTReplayBase *, struct Module * Module, BYTE Channels);
void _PTReplay_PTOffChannel(struct PTReplayBase *, struct Module * Module, BYTE Channels);
void _PTReplay_PTSetPos(struct PTReplayBase *, struct Module * Module, UBYTE Pos);
void _PTReplay_PTSetPri(struct PTReplayBase *, BYTE Pri);
BYTE _PTReplay_PTGetPri(struct PTReplayBase *);
BYTE _PTReplay_PTGetChan(struct PTReplayBase *);
struct PTSample *_PTReplay_PTGetSample(struct PTReplayBase *, struct Module * Module, WORD Nr);

struct Library *PTReplayBase;
struct ExecBase *SysBase;
struct DosLibrary *DOSBase;
struct Library *PtPlayBase;

/**********************************************************************
	LibInit
**********************************************************************/

STATIC struct Library *LibInit(REG(d0, struct PTReplayBase *libbase), REG(a0, BPTR seglist), REG(a6, struct ExecBase *mysysbase))
{
	libbase->segList = seglist;
	SysBase = mysysbase;
	PTReplayBase = (APTR)libbase;

	InitSemaphore(&libbase->semaphore);

	if ((DOSBase = (APTR)OpenLibrary("dos.library", 36)))
	{
		return &libbase->libNode;
	}
	else
	{
		FreeMem((APTR)((ULONG)(libbase) - (ULONG)(libbase->libNode.lib_NegSize)), libbase->libNode.lib_NegSize + libbase->libNode.lib_PosSize);
		return NULL;
	}
}

/**********************************************************************
	LibExpunge
**********************************************************************/

STATIC BPTR LibExpunge(struct PTReplayBase *libbase)
{
	BPTR seglist = 0;

	libbase->libNode.lib_Flags |= LIBF_DELEXP;

	if (libbase->libNode.lib_OpenCnt == 0)
	{
		REMOVE(&libbase->libNode.lib_Node);

		seglist = libbase->segList;

		CloseLibrary((struct Library *)DOSBase);
		FreeMem((APTR)((ULONG)(libbase) - (ULONG)(libbase->libNode.lib_NegSize)), libbase->libNode.lib_NegSize + libbase->libNode.lib_PosSize);
	}

	return seglist;
}

/**********************************************************************
	LibOpen
**********************************************************************/

STATIC struct Library *LibOpen(struct PTReplayBase *libbase)
{
	APTR base = libbase;

	libbase->libNode.lib_OpenCnt++;
	libbase->libNode.lib_Flags &= ~LIBF_DELEXP;

	if (PtPlayBase == NULL)
	{
		ObtainSemaphore(&libbase->semaphore);

		if (PtPlayBase == NULL)
		{
			PtPlayBase = OpenLibrary("ptplay.library", 0);

			if (PtPlayBase == NULL)
			{
				Printf("Unable to open ptplay.library\n");
				libbase->libNode.lib_OpenCnt--;
				base = NULL;
			}
		}

		ReleaseSemaphore(&libbase->semaphore);
	}

	return base;
}

/**********************************************************************
	LibClose
**********************************************************************/

STATIC BPTR LibClose(struct PTReplayBase *libbase)
{
	BPTR seglist = 0;

	libbase->libNode.lib_OpenCnt--;

	if (libbase->libNode.lib_OpenCnt == 0)
	{
		ObtainSemaphore(&libbase->semaphore);

		if (libbase->libNode.lib_OpenCnt == 0)
		{
			CloseLibrary(PtPlayBase);

			PtPlayBase = NULL;
		}

		ReleaseSemaphore(&libbase->semaphore);

		if (libbase->libNode.lib_OpenCnt == 0 && libbase->libNode.lib_Flags & LIBF_DELEXP)
		{
			seglist = LibExpunge(libbase);
		}
	}

	return seglist;
}

STATIC ULONG LIB_Reserved(void)
{
	return 0;
}

#if defined(__MORPHOS__)
STATIC struct Library *LIB_Open(void)
{
	return LibOpen((APTR)REG_A6);
}

STATIC BPTR LIB_Close(void)
{
	return LibClose((APTR)REG_A6);
}

STATIC BPTR LIB_Expunge(void)
{
	return LibExpunge((APTR)REG_A6);
}

STATIC struct Module * stub_PTLoadModule(void)
{
	return _PTReplay_PTLoadModule((APTR)REG_A6, (APTR)REG_A0);
}

STATIC VOID stub_PTUnloadModule(void)
{
	_PTReplay_PTUnloadModule((APTR)REG_A6, (APTR)REG_A0);
}

STATIC ULONG stub_PTPlay(void)
{
	return _PTReplay_PTPlay((APTR)REG_A6, (APTR)REG_A0);
}

STATIC ULONG stub_PTStop(void)
{
	return _PTReplay_PTStop((APTR)REG_A6, (APTR)REG_A0);
}

STATIC ULONG stub_PTPause(void)
{
	return _PTReplay_PTPause((APTR)REG_A6, (APTR)REG_A0);
}

STATIC ULONG stub_PTResume(void)
{
	return _PTReplay_PTResume((APTR)REG_A6, (APTR)REG_A0);
}

STATIC VOID stub_PTFade(void)
{
	_PTReplay_PTFade((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC VOID stub_PTSetVolume(void)
{
	_PTReplay_PTSetVolume((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC UBYTE stub_PTSongPos(void)
{
	return _PTReplay_PTSongPos((APTR)REG_A6, (APTR)REG_A0);
}

STATIC UBYTE stub_PTSongLen(void)
{
	return _PTReplay_PTSongLen((APTR)REG_A6, (APTR)REG_A0);
}

STATIC UBYTE stub_PTSongPattern(void)
{
	return _PTReplay_PTSongPattern((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC UBYTE stub_PTPatternPos(void)
{
	return _PTReplay_PTPatternPos((APTR)REG_A6, (APTR)REG_A0);
}

STATIC APTR stub_PTPatternData(void)
{
	return _PTReplay_PTPatternData((APTR)REG_A6, (APTR)REG_A0, REG_D0, REG_D1);
}

STATIC void stub_PTInstallBits(void)
{
	_PTReplay_PTInstallBits((APTR)REG_A6, (APTR)REG_A0, REG_D0, REG_D1, REG_D2, REG_D3);
}

STATIC struct Module * stub_PTSetupMod(void)
{
	return _PTReplay_PTSetupMod((APTR)REG_A6, (APTR)REG_A0);
}

STATIC void stub_PTFreeMod(void)
{
	_PTReplay_PTFreeMod((APTR)REG_A6, (APTR)REG_A0);
}

STATIC void stub_PTStartFade(void)
{
	_PTReplay_PTStartFade((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC void stub_PTOnChannel(void)
{
	_PTReplay_PTOnChannel((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC void stub_PTOffChannel(void)
{
	_PTReplay_PTOffChannel((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC void stub_PTSetPos(void)
{
	_PTReplay_PTSetPos((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}

STATIC void stub_PTSetPri(void)
{
	_PTReplay_PTSetPri((APTR)REG_A6, REG_D0);
}

STATIC BYTE stub_PTGetPri(void)
{
	return _PTReplay_PTGetPri((APTR)REG_A6);
}

STATIC BYTE stub_PTGetChan(void)
{
	return _PTReplay_PTGetChan((APTR)REG_A6);
}

STATIC struct PTSample * stub_PTGetSample(void)
{
	return _PTReplay_PTGetSample((APTR)REG_A6, (APTR)REG_A0, REG_D0);
}
#else
STATIC struct Library *LIB_Open(MREG(a6, struct PTReplayBase *libbase))
{
	return LibOpen(libbase);
}

STATIC BPTR LIB_Close(MREG(a6, struct PTReplayBase *libbase))
{
	return LibClose(libbase);
}

STATIC BPTR LIB_Expunge(MREG(a6, struct PTReplayBase *libbase))
{
	return LibExpunge(libbase);
}

STATIC struct Module * stub_PTLoadModule(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTLoadModule(libbase, module);
}

STATIC VOID stub_PTUnloadModule(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	_PTReplay_PTUnloadModule(libbase, module);
}

STATIC ULONG stub_PTPlay(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTPlay(libbase, module);
}

STATIC ULONG stub_PTStop(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTStop(libbase, module);
}

STATIC ULONG stub_PTPause(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTPause(libbase, module);
}

STATIC ULONG stub_PTResume(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTResume(libbase, module);
}

STATIC VOID stub_PTFade(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTFade(libbase, module, REG_D0);
}

STATIC VOID stub_PTSetVolume(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTSetVolume(libbase, module, REG_D0);
}

STATIC UBYTE stub_PTSongPos(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTSongPos(libbase, module);
}

STATIC UBYTE stub_PTSongLen(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTSongLen(libbase, module);
}

STATIC UBYTE stub_PTSongPattern(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	return _PTReplay_PTSongPattern(libbase, module, REG_D0);
}

STATIC UBYTE stub_PTPatternPos(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTPatternPos(libbase, module);
}

STATIC APTR stub_PTPatternData(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0), MREG(d1, ULONG REG_D1))
{
	return _PTReplay_PTPatternData(libbase, module, REG_D0, REG_D1);
}

STATIC void stub_PTInstallBits(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0), MREG(d1, ULONG REG_D1), MREG(d2, ULONG REG_D2), MREG(d3, ULONG REG_D3))
{
	_PTReplay_PTInstallBits(libbase, module, REG_D0, REG_D1, REG_D2, REG_D3);
}

STATIC struct Module * stub_PTSetupMod(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	return _PTReplay_PTSetupMod(libbase, module);
}

STATIC void stub_PTFreeMod(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module))
{
	_PTReplay_PTFreeMod(libbase, module);
}

STATIC void stub_PTStartFade(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTStartFade(libbase, module, REG_D0);
}

STATIC void stub_PTOnChannel(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTOnChannel(libbase, module, REG_D0);
}

STATIC void stub_PTOffChannel(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTOffChannel(libbase, module, REG_D0);
}

STATIC void stub_PTSetPos(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTSetPos(libbase, module, REG_D0);
}

STATIC void stub_PTSetPri(MREG(a6, struct PTReplayBase *libbase), MREG(d0, ULONG REG_D0))
{
	_PTReplay_PTSetPri(libbase, REG_D0);
}

STATIC BYTE stub_PTGetPri(MREG(a6, struct PTReplayBase *libbase))
{
	return _PTReplay_PTGetPri(libbase);
}

STATIC BYTE stub_PTGetChan(MREG(a6, struct PTReplayBase *libbase))
{
	return _PTReplay_PTGetChan(libbase);
}

STATIC struct PTSample * stub_PTGetSample(MREG(a6, struct PTReplayBase *libbase), MREG(a0, APTR module), MREG(d0, ULONG REG_D0))
{
	return _PTReplay_PTGetSample(libbase, module, REG_D0);
}
#endif

STATIC CONST APTR FuncTable[] =
{
	#if defined(__MORPHOS__)
	(APTR)	FUNCARRAY_32BIT_NATIVE, 
	#endif

	(APTR) LIB_Open,
	(APTR) LIB_Close,
	(APTR) LIB_Expunge,
	(APTR) LIB_Reserved,
	(APTR) &stub_PTLoadModule,
	(APTR) &stub_PTUnloadModule,
	(APTR) &stub_PTPlay,
	(APTR) &stub_PTStop,
	(APTR) &stub_PTPause,
	(APTR) &stub_PTResume,
	(APTR) &stub_PTFade,
	(APTR) &stub_PTSetVolume,
	(APTR) &stub_PTSongPos,
	(APTR) &stub_PTSongLen,
	(APTR) &stub_PTSongPattern,
	(APTR) &stub_PTPatternPos,
	(APTR) &stub_PTPatternData,
	(APTR) &stub_PTInstallBits,
	(APTR) &stub_PTSetupMod,
	(APTR) &stub_PTFreeMod,
	(APTR) &stub_PTStartFade,
	(APTR) &stub_PTOnChannel,
	(APTR) &stub_PTOffChannel,
	(APTR) &stub_PTSetPos,
	(APTR) &stub_PTSetPri,
	(APTR) &stub_PTGetPri,
	(APTR) &stub_PTGetChan,
	(APTR) &stub_PTGetSample,
	(APTR) -1
};

STATIC CONST struct MyInitData InitData	=
{
	/* This is fucked way of coding but I can not let old habits go :-) */
	{ 0xa0,  8, NT_LIBRARY, 0 },
	{ 0xa0,  9, 0       , 0 },
	{ 0x80, 10 }, (ULONG)&libname,
	{ 0xa0, 14, LIBF_SUMUSED|LIBF_CHANGED, 0 },
	{ 0x90, 20 }, VERSION,
	{ 0x90, 22 }, REVISION,
	{ 0x80, 24 }, (ULONG)&vstring,
	0
};

STATIC CONST ULONG InitTable[] =
{
	sizeof(struct PTReplayBase),
	(ULONG)	FuncTable,
	(ULONG)	&InitData,
	(ULONG)	LibInit
};

CONST struct Resident __TEXTSEGMENT__ RomTag =
{
	RTC_MATCHWORD,
	(struct Resident *)&RomTag,
	(struct Resident *)&RomTag+1,
	#if defined(__MORPHOS__)
	RTF_AUTOINIT | RTF_PPC | RTF_EXTENDED,
	#else
	RTF_AUTOINIT,
	#endif
	VERSION,
	NT_LIBRARY,
	0,
	(char *)libname,
	(char *)vstring,
	(APTR)InitTable

	#if defined(__MORPHOS__)
	, REVISION, NULL
	#endif
};
