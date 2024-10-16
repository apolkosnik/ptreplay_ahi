#include "ptreplay_private.h"

/* Version Tag */
#include "ptreplay.library_rev.h"
CONST UBYTE USED verstag[] = VERSTAG;

struct Library *DOSBase;
struct Library *PtPlayBase;
struct ExecIFace *IExec;
struct DOSIFace *IDOS;
struct PtPlayIFace *IPtPlay;

STATIC int32 OpenLibs ();
STATIC void CloseLibs ();

/*
 * The system (and compiler) rely on a symbol named _start which marks
 * the beginning of execution of an ELF file. To prevent others from 
 * executing this library, and to keep the compiler/linker happy, we
 * define an empty _start symbol here.
 *
 * On the classic system (pre-AmigaOS4) this was usually done by
 * moveq #0,d0
 * rts
 *
 */
int32 _start(void) {
    /* If you feel like it, open DOS and print something to the user */
    return RETURN_FAIL;
}

/* Open the library */
STATIC struct PTReplayBase *libOpen(struct LibraryManagerInterface *Self, ULONG version) {
    struct PTReplayBase *libBase = (struct PTReplayBase *)Self->Data.LibBase; 

    if (version > VERSION) {
        return NULL;
    }

    /* Add any specific open code here 
       Return 0 before incrementing OpenCnt to fail opening */

    /* Add up the open count */
    libBase->libNode.lib_OpenCnt++;
    return libBase;
}

/* Close the library */
STATIC BPTR libClose(struct LibraryManagerInterface *Self) {
    struct PTReplayBase *libBase = (struct PTReplayBase *)Self->Data.LibBase;
    /* Make sure to undo what open did */

    /* Make the close count */
    libBase->libNode.lib_OpenCnt--;

    return ZERO;
}

/* Expunge the library */
STATIC BPTR libExpunge(struct LibraryManagerInterface *Self) {
    /* If your library cannot be expunged, return 0 */
    BPTR result = ZERO;
    struct PTReplayBase *libBase = (struct PTReplayBase *)Self->Data.LibBase;
    if (libBase->libNode.lib_OpenCnt == 0) {
	    result = libBase->segList;
        /* Undo what the init code did */
        CloseLibs();

        IExec->Remove((struct Node *)libBase);
        IExec->DeleteLibrary((struct Library *)libBase);
    } else {
        result = ZERO;
        libBase->libNode.lib_Flags |= LIBF_DELEXP;
    }
    return result;
}

/* The ROMTAG Init Function */
STATIC struct PTReplayBase *libInit(struct PTReplayBase *libBase, BPTR seglist, struct ExecIFace *exec) {
    IExec = exec;
    libBase->libNode.lib_Node.ln_Type = NT_LIBRARY;
    libBase->libNode.lib_Node.ln_Pri  = 0;
    libBase->libNode.lib_Node.ln_Name = "ptreplay.library";
    libBase->libNode.lib_Flags        = LIBF_SUMUSED|LIBF_CHANGED;
    libBase->libNode.lib_Version      = VERSION;
    libBase->libNode.lib_Revision     = REVISION;
    libBase->libNode.lib_IdString     = VSTRING;
    libBase->segList = seglist;

	if (!OpenLibs()) {
		CloseLibs();
		return NULL;
	}

    return libBase;
}

STATIC int32 OpenLibs () {
	DOSBase = IExec->OpenLibrary("dos.library", 52);
	IDOS = (struct DOSIFace *)IExec->GetInterface(DOSBase, "main", 1, NULL);
	if (!IDOS) return FALSE;
	PtPlayBase = IExec->OpenLibrary("ptplay.library", 0);
	IPtPlay = (struct PtPlayIFace *)IExec->GetInterface(PtPlayBase, "main", 1, NULL);
	if (!IPtPlay) return FALSE;
	return TRUE;
}

STATIC void CloseLibs () {
	IExec->DropInterface((struct Interface *)IPtPlay);
	IExec->CloseLibrary(PtPlayBase);
	IExec->DropInterface((struct Interface *)IDOS);
	IExec->CloseLibrary(DOSBase);
}

/* ------------------- Manager Interface ------------------------ */
/* These are generic. Replace if you need more fancy stuff */
STATIC uint32 _manager_Obtain(struct LibraryManagerInterface *Self) {
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

STATIC uint32 _manager_Release(struct LibraryManagerInterface *Self) {
	uint32 res;
	__asm__ __volatile__(
	"1:	lwarx	%0,0,%1\n"
	"addic	%0,%0,-1\n"
	"stwcx.	%0,0,%1\n"
	"bne-	1b"
	: "=&r" (res)
	: "r" (&Self->Data.RefCount)
	: "cc", "memory");

	return res;
}

/* Manager interface vectors */
STATIC CONST APTR lib_manager_vectors[] = {
	_manager_Obtain,
	_manager_Release,
	NULL,
	NULL,
	libOpen,
	libClose,
	libExpunge,
	NULL,
	(APTR)-1
};

/* "__library" interface tag list */
STATIC CONST struct TagItem lib_managerTags[] = {
	{ MIT_Name,			(Tag)"__library"		},
	{ MIT_VectorTable,	(Tag)lib_manager_vectors},
	{ MIT_Version,		1						},
	{ TAG_DONE,			0						}
};

/* ------------------- Library Interface(s) ------------------------ */

#include "ptreplay_vectors.c"

/* Uncomment this line (and see below) if your library has a 68k jump table */
extern APTR VecTable68K[];

STATIC CONST struct TagItem mainTags[] = {
	{ MIT_Name,			(Tag)"main"			},
	{ MIT_VectorTable,	(Tag)main_vectors	},
	{ MIT_Version,		1					},
	{ TAG_DONE,			0					}
};

STATIC CONST CONST_APTR libInterfaces[] = {
	lib_managerTags,
	mainTags,
	NULL
};

STATIC CONST struct TagItem libCreateTags[] = {
	{ CLT_DataSize,		sizeof(struct PTReplayBase)	},
	{ CLT_InitFunc,		(Tag)libInit			},
	{ CLT_Interfaces,	(Tag)libInterfaces		},
	/* Uncomment the following line if you have a 68k jump table */
	{ CLT_Vector68K, (Tag)VecTable68K },
	{TAG_DONE,		 0 }
};

/* ------------------- ROM Tag ------------------------ */
CONST struct Resident USED lib_res = {
	RTC_MATCHWORD,
	(struct Resident *)&lib_res,
	(APTR)(&lib_res + 1),
	RTF_NATIVE|RTF_AUTOINIT, /* Add RTF_COLDSTART if you want to be resident */
	VERSION,
	NT_LIBRARY, /* Make this NT_DEVICE if needed */
	0, /* PRI, usually not needed unless you're resident */
	"ptreplay.library",
	VSTRING,
	(APTR)libCreateTags
};
