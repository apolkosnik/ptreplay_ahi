/****************************************

 PT-Player 2.0   � 1994 BetaSoft
 
 uses PTReplay library also by BetaSoft
 and Andreas [Pucko] P�lsson.
 
*****************************************/

#include <exec/types.h>
#include <dos/dos.h>
#include <libraries/asl.h>

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/asl_protos.h>

#include <pragmas/exec_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/gadtools_pragmas.h>
#include <pragmas/asl_pragmas.h>

#include <string.h>

#include "control.h"
//#include "include/ptreplay_protos.h"
//#include "include/ptreplay_pragmas.h"
#include "include/ptreplay.h"

struct Library *PTReplayBase;

struct Module *Mod = NULL;
struct FileRequester *FileReq = NULL;

STRPTR vstr = "$VER: PT-Player 2.1 ";

BYTE StopBit,PatternBit,RowBit;
BOOL Loop=FALSE,Paused=FALSE;

struct TagItem FRTags[]=
{
	ASLFR_SleepWindow, TRUE,
	ASLFR_TitleText, (ULONG)"Load Module",
	ASLFR_PositiveText, (ULONG)"Load",
	ASLFR_InitialPattern, (ULONG)"(mod.#?|#?.mod)",
	ASLFR_DoPatterns,TRUE,
	ASLFR_RejectIcons, TRUE,
	TAG_DONE
};

void MainRoutine(void)
{
BOOL Running=TRUE;
ULONG Flags,WinMask,StopMask,PatternMask,RowMask;
UBYTE Pos, Pat, Row;

	WinMask=1L<<ControlWnd->UserPort->mp_SigBit;
	StopMask=1L<<StopBit;
	PatternMask=1L<<PatternBit;
	RowMask=1L<<RowBit;

	while(Running)
	{
		Flags=Wait(WinMask|StopMask|PatternMask|RowMask);

		if(Flags & WinMask)
			Running=HandleControlIDCMP();

		if(Flags & StopMask)
			if(!Loop)
				StopClicked();

		if(Flags & (PatternMask|RowMask))
		{
			Pos=PTSongPos(Mod);

			GT_SetGadgetAttrs(ControlGadgets[GD_Pos], ControlWnd, NULL,
								GTNM_Number, Pos, TAG_DONE);
			Pat=PTSongPattern(Mod,Pos);
			GT_SetGadgetAttrs(ControlGadgets[GD_Pat], ControlWnd, NULL,
								GTNM_Number, Pat, TAG_DONE);
			Row=PTPatternPos(Mod);
			GT_SetGadgetAttrs(ControlGadgets[GD_Row], ControlWnd, NULL,
								GTNM_Number, Row, TAG_DONE);
		}
	}
}

int main(int argc, char **argv)
{
	if(PTReplayBase = OpenLibrary("ptreplay.library",4L))
	{
		if(FileReq = AllocAslRequest(ASL_FileRequest, FRTags))
		{
			if(!SetupScreen())
			{
				if(!OpenControlWindow())
				{
					StopBit=AllocSignal(-1);
					PatternBit=AllocSignal(-1);
					RowBit=AllocSignal(-1);

					if((StopBit != -1)&(PatternBit != -1)&(RowBit != -1))
						MainRoutine();

					FreeSignal(StopBit);
					FreeSignal(PatternBit);
					FreeSignal(RowBit);
				}
				CloseControlWindow();
			}
			CloseDownScreen();

			FreeAslRequest(FileReq);
		}
		CloseLibrary(PTReplayBase);
	}
	return(0l);
}
