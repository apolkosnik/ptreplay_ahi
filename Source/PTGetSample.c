#include "ptreplay_private.h"
#include "ptplay_priv.h"

struct PTSample * _PTReplay_PTGetSample(struct PTReplayIFace *Self,
	struct Module *module, WORD Nr)
{
	/* Write me. Really, I dare you!
	
   NAME   
        PTGetSample -- Return info on a sample. (V6)

   SYNOPSIS
        Sam=PTGetSample(Module,Num);
        D0              A0     D0

        struct PTSample *PTGetSample(struct Module *,WORD);

   FUNCTION
        This function collects info about the given sample-number.

        it returns the folowing structure:
        struct PTSample
        {
            UBYTE Name[22];    // Null terminated string with samplename 
            UWORD Length;      // Sample length in words 
            UBYTE FineTune;    // FineTune of sample in lower 4 bits 
            UBYTE Volume;      // Volume of sample 
            UWORD Repeat;      // Repeat start in number of words 
            UWORD Replen;      // Repeat length in number of words 
        };

   INPUTS
        Module - Pointer to a loaded module
        Num    - Number of the sample you whant info about. MUST BE 0-31

   RESULT
        Sam - Info about the given sample.

   SEE ALSO
	 */

	//IExec->DebugPrintF("Function ptreplay::PTGetSample not implemented\n"); 
	dbug(("ptreplay::PTGetSample TESTME!\n"));

	pt_sample_s		*s;
	pt_mod_s	*mod;
	Nr = Nr & 31;
	ObtainSemaphore(&module->mutex);
	mod = (pt_mod_s *)module->mod_handle;
	s = (pt_sample_s *)&mod->sample[Nr];
	ReleaseSemaphore(&module->mutex);
	
	return s;
	}
