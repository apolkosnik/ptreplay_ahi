
#include <exec/types.h>
#include <utility/tagitem.h>
#include <libraries/asl.h>
#include <libraries/gadtools.h>
#include <intuition/intuition.h>
#include <string.h>
#include <stdio.h>

#include <clib/dos_protos.h>
#include <clib/asl_protos.h>
#include <clib/gadtools_protos.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/asl_pragmas.h>
#include <pragmas/gadtools_pragmas.h>

#include "control.h"
//#include "include/ptreplay_protos.h"
//#include "include/ptreplay_pragmas.h"
#include "include/ptreplay.h"

int PlayClicked( void )
{
            if(Mod) {
                UBYTE Pos,Pat,Row;
            	/* routine when gadget "_Play" is clicked. */
            	PTPlay(Mod);

            	/* refresh the Song position and Pattern number */
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
	return(TRUE);
}

int StopClicked( void )
{
UBYTE Pos,Pat,Row;
	
	
	if(Mod){
            	/* routine when gadget "_Stop" is clicked. */
            	PTStop(Mod);

            	/* refresh the Song position and Pattern number */
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
	return(TRUE);
}

int QuitClicked( void )
{
	/* routine when gadget "_Quit" is clicked. */
	if(Mod)
	{
		PTStop(Mod);
		PTUnloadModule(Mod);
	}
	return(FALSE);
}

int PauseClicked( void )
{
UBYTE Row;
	
	if(Mod){
            	/* routine when gadget "P_ause" is clicked. */
            	if(Paused)
            		PTResume(Mod);
            	else
            		PTPause(Mod);

            	Paused=~Paused;
            	Row=PTPatternPos(Mod);
            	GT_SetGadgetAttrs(ControlGadgets[GD_Row], ControlWnd, NULL,
						GTNM_Number, Row, TAG_DONE);
	}
	return(TRUE);
}

int LoadClicked( void )
{
char Buffer[512];
UBYTE Len,Pos,Pat,Row;
//APTR *RowData;
struct Module *Modz;



	PTStop(Mod);

	if(AslRequestTags(FileReq, ASLFR_Window, ControlWnd,TAG_DONE))
	{
		strcpy(Buffer, FileReq->rf_Dir);
		AddPart(Buffer, FileReq->fr_File, 256);

		if(Mod=PTLoadModule(Buffer))
		{
			PTInstallBits(Mod, StopBit, PatternBit, RowBit, -1);
			
			GT_SetGadgetAttrs(ControlGadgets[GD_Module], ControlWnd, NULL,
								GTTX_Text, FileReq->fr_File, TAG_DONE);
			Len=PTSongLen(Mod);
			GT_SetGadgetAttrs(ControlGadgets[GD_Length], ControlWnd, NULL,
								GTNM_Number, Len, TAG_DONE );
			Pos=PTSongPos(Mod);
			GT_SetGadgetAttrs(ControlGadgets[GD_Pos], ControlWnd, NULL,
								GTNM_Number, Pos, TAG_DONE);
			Pat=PTSongPattern(Mod,Pos);
			GT_SetGadgetAttrs(ControlGadgets[GD_Pat], ControlWnd, NULL,
								GTNM_Number, Pat, TAG_DONE);
			Row=PTPatternPos(Mod);
			GT_SetGadgetAttrs(ControlGadgets[GD_Row], ControlWnd, NULL,
								GTNM_Number, Row, TAG_DONE);
			//RowData=PTPatternData(Mod,Pat,Row);
			GT_SetGadgetAttrs(ControlGadgets[GD_RData], ControlWnd, NULL,
								GTTX_Text, "--", TAG_DONE);

		}
	}

	return(TRUE);
}

int FadeClicked( void )
{
            if(Mod){
            	/* routine when gadget "_Fade" is clicked. */
            	PTStartFade(Mod,1);
            }
	return(TRUE);
}

int LoopClicked( void )
{

	
	/* routine when gadget "L_oop" is clicked. */
	Loop=ControlGadgets[GD_Loop]->Flags & GFLG_SELECTED;
	return(TRUE);
}

int ControlCloseWindow( void )
{
	/* routine for "IDCMP_CLOSEWINDOW". */
	if(Mod)
	{
		PTStop(Mod);
		PTUnloadModule(Mod);
	}
	return(FALSE);
}

int ControlVanillaKey( void )
{
 ULONG    ltick;
	/* routine for "IDCMP_VANILLAKEY". */
	switch(ControlMsg.Code)
	{

		case 'o':
		case 'O':
                               
                                GT_GetGadgetAttrs(ControlGadgets[GD_Loop], ControlWnd, NULL,
            					GTCB_Checked, &ltick, TAG_DONE);
                                if(ltick==TRUE){ ltick=FALSE;}
                                else { ltick=TRUE; }            
                                GT_SetGadgetAttrs(ControlGadgets[GD_Loop], ControlWnd, NULL,
						GTCB_Checked, ltick, TAG_DONE);
			//LoopClicked();
			break;

		case 'p':
		case 'P':
			PlayClicked();
			break;

		case 's':
		case 'S':
			StopClicked();
			break;

		case 'q':
		case 'Q':
			QuitClicked();
			break;

		case 'a':
		case 'A':
			PauseClicked();
			break;

		case 'l':
		case 'L':
			LoadClicked();
			break;

		case 'f':
		case 'F':
			FadeClicked();
			break;
	}
	return(TRUE);
}
