#ifndef PTREPLAY_PRIVATE_H
#define PTREPLAY_PRIVATE_H

#include <exec/exec.h>
#include <dos/dos.h>
#include <devices/ahi.h>
#include <libraries/ptplay.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/ptplay.h>
#include <proto/ptreplay.h>

#ifdef DEBUG
#define dbug(x) IExec->DebugPrintF x
#else
#define dbug(x)
#endif

#if !defined(__amigaos4__)
typedef BYTE int8;
typedef LONG int32;
typedef ULONG uint32;
#endif

/* Long word alignement (mainly used to get
 * FIB or DISK_INFO as auto variables)
 */
#define D_S(type,name) char a_##name[sizeof(type)+3]; \
                       type *name = (type *)((IPTR)(a_##name+3) & ~3)

#define PTReplayIFace PTReplayBase

struct PTReplayBase {
	struct Library libNode;
	BPTR segList;

	#if !defined(__amigaos4__)
	struct SignalSemaphore semaphore;
	#endif
};

enum {
	PTCMD_DIE = 0,
	PTCMD_STARTUP,
	PTCMD_PLAY,
	PTCMD_STOP,
	PTCMD_PAUSE,
	PTCMD_RESUME,
	PTCMD_FADE,
	PTCMD_STARTFADE
};

enum {
	PTSIG_RESTART = 0,
	PTSIG_POS,
	PTSIG_ROW,
	PTSIG_FADE,
	PTSIG_MAX
};

struct PTReplayMessage {
	struct Message ptm_Msg;
	struct Module *ptm_Module;
	int32 ptm_Command;
	uint32 *ptm_Args;
	struct Message death_msg;
};

struct Module {
	APTR file_data;
	int32 file_size;
	int32 free_file_data;
	struct SignalSemaphore mutex;
	uint32 mod_type;
	APTR mod_handle;
	struct MsgPort reply_port;
	struct MsgPort player_port;
	struct PTReplayMessage cmd_msg;
	struct Process *parent_proc;
	struct Process *player_proc;
	int8 signals[PTSIG_MAX];
	uint32 ptplay_flags;
};

#if defined(__PPC__)
#define __TEXTSEGMENT__ __attribute__((section(".text")))
#else
#define __TEXTSEGMENT__
#endif

#if !defined(__PPC__) && defined(__VBCC__)
#define __attribute__(x)
#endif

struct MyInitData
{
	UBYTE ln_Type_Init[4];
	UBYTE ln_Pri_Init[4];
	UBYTE ln_Name_Init[2];
	ULONG __attribute__((aligned(2)))	ln_Name_Content;
	UBYTE lib_Flags_Init[4];
	UBYTE lib_Version_Init[2]; UWORD lib_Version_Content;
	UBYTE lib_Revision_Init[2]; UWORD lib_Revision_Content;
	UBYTE lib_IdString_Init[2];
	ULONG  __attribute__((aligned(2)))	lib_IdString_Content;
	UWORD EndMark;
} __attribute__((packed));

/* ptreplay_private.c */
struct Module *_PTSetupModInternal(struct Module *module, STRPTR name);
void _PTFreeModInternal(struct Module *module);
void _PTSendCommand(struct Module *module, ULONG command, ...);

/* player_main.c */
void player_main ();

/* player */
#define PLAYER_BUF_SIZE (44100/10*4)

/* library */
#ifdef MASM
#  undef MASM
#endif
#ifdef MREG
#  undef MREG
#endif

#if defined(__SASC)
#  define MASM __asm
#  define MREG(reg,type) register __##reg type
#elif defined(__GNUC__)
#  define MASM
#  define MREG(reg,type) type __asm(#reg)
#elif defined(__VBCC__)
#  define MASM
#  define MREG(reg,type) __reg(#reg) type
#elif defined(__MAXON__) | defined(__STORM__)
#  define MASM
#  define MREG(reg,type) register __##reg type
#else
#  error Unknown compiler, SAS/C, GCC, VBCC, MaxonC and StormC supported
#endif

#if defined(__MORPHOS__)
#define REG(reg,arg) arg
#else
#define REG(reg,arg) MREG(reg,arg)
#endif

#if !defined(__MORPHOS__)
# undef REMOVE
# define REMOVE Remove
#endif

#endif
