#include "ptreplay_private.h"

void _PTReplay_PTOffChannel(struct PTReplayIFace *Self,
	struct Module *module, BYTE Channels)
{
	/* Write me. Really, I dare you! 
	
   NAME   
        PTOffChannel -- Turns off one or more channels. (V5)

   SYNOPSIS
        PTStartFade(Module, Channel)
                    A0      D0

        void PTStartFade(struct Module *, BYTE);

   FUNCTION
        This function is used to turn one or more channels off in order to
        stop ptreplay producing sound for that channel. Note that this isn't
        a nested command and turning off a channel that is already off will
        give you strange results and is _not_ recomended.

   INPUTS
        Module - Module to select channels for.
        Channel - Channels to be turned off.

   RESULT

   EXAMPLE

   NOTES

   BUGS
        Turning a channel off will not release it to be used by other
        programms via audio.device.

   SEE ALSO
        PTOnChannel()

	
	
	
	*/
	//IExec->DebugPrintF("Function ptreplay::PTOffChannel not implemented\n");
}
