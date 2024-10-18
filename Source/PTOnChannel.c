#include "ptreplay_private.h"

void _PTReplay_PTOnChannel(struct PTReplayIFace *Self,
	struct Module *module, BYTE Channels)
{
	/* Write me. Really, I dare you! 
	
   NAME   
        PTOnChannel -- Turns on one or more channels. (V5)

   SYNOPSIS
        PTStartFade(Module, Channel)
                    A0      D0

        void PTStartFade(struct Module *, BYTE);

   FUNCTION
        This function is used to turn one or more channels on in order for
        ptreplay to produce sound for that channel. Note that this isn't
        a nested command and turning on a channel that is already on will
        give you strange results.

   EXAMPLE
       ;Turn on all 4 channels
       PTOnChannel(Mod, $f);

       ;Turn on channel 0,1 & 2
       PTOnChannel(Mod, &7);

   INPUTS
        Module - Module to select channels for.
        Channel - Channels to be turned on.

   SEE ALSO
        PTOffChannel()

	
	
	*/
	//IExec->DebugPrintF("Function ptreplay::PTOnChannel not implemented\n");
}
