#include "ptreplay_private.h"

VOID _PTReplay_PTFade(struct PTReplayIFace *Self,
	struct Module *module, UBYTE speed)
{
	dbug(("ptreplay::PTFade\n"));

	_PTSendCommand(module, PTCMD_FADE, speed);
}
