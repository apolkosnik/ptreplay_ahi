#include "ptreplay_private.h"
#include "ptplay_priv.h"

APTR _PTReplay_PTPatternData(struct PTReplayIFace *Self,
	struct Module *module, UBYTE Pattern, UBYTE Row)
{
	/* Write me. Really, I dare you! 
	
   NAME
        PTPatternData -- Returns the adress of specified pattern/row (V4)

   SYNOPSIS
        RowData=PTPatternData(Module, PatternNum, RowNum)
        D0                    A0      D0          D1

        APTR PTPatternData(struct Module *, UBYTE, UBYTE);

   FUNCTION
        This function returns a pointer to the given row in the specified
        pattern.

   INPUTS
        Module     - Pointer to the module you which to examine.
        PatternNum - The pattern (0-127) you want to examine.
        RowNum      - And finaly which row (0-63) to examine.

   RESULT
        RowData - A pointer to the given position


	
	
	*/

	dbug(("ptreplay::PTPatternData TESTME!\n"));

    pt_mod_s* mod;
	pt_pattern_s *p;
	APTR *ptd;

    if ((Pattern <= 127) && (Row <= 63)) {
		ObtainSemaphore(&module->mutex);
		//PtGetAttr(module->mod_handle, PTPLAY_PatternData, &res);



		mod = (pt_mod_s *)module->mod_handle;

		p = mod->pattern + mod->pos[Pattern];		/* pattern */

		ptd = p->data[Row];							/* patterndata */

		ReleaseSemaphore(&module->mutex);
    }

	return ptd;

}
