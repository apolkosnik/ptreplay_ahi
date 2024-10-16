#include "ptreplay_private.h"

ULONG _PTReplay_PTPlay(struct PTReplayIFace *Self,
	struct Module *module)
{
	dbug(("ptreplay::PTPlay\n"));

	_PTSendCommand(module, PTCMD_PLAY);
	return TRUE;
}
