#include "ptreplay_private.h"
#include "ptplay_priv.h"

UBYTE _PTReplay_PTSongPattern(struct PTReplayIFace *Self,
	struct Module *module, UWORD Pos)
{

/*
   NAME   
        PTSongPattern -- Returns pattern at a given position (V4)

   SYNOPSIS
        PatNum=PTSongPattern(module, pos)
        D0                   A0      D0

        UBYTE PTSongPattern(struct Module *,UWORD);

   FUNCTION
        Returns the pattern number at a given position.

   INPUTS
        Module - The module you want to examine
        Pos - The position (0-127) you want to examine.

   RESULT
        PatNum - A number (0-127) telling you which pattern is to be played.



*/


	dbug(("ptreplay::PTSongPattern TESTME!\n"));

     UBYTE res = (UBYTE)0; 
     pt_mod_s* mod;

     if (Pos <= 127) {
	     ObtainSemaphore(&module->mutex);
          mod = (pt_mod_s *)module->mod_handle;
          res = (UBYTE)mod->pos[Pos];
          //res = (UBYTE)mod->modformat;	
		ReleaseSemaphore(&module->mutex);
     }

	return (UBYTE)res;

}
