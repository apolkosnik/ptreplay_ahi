#include "ptreplay_private.h"

ULONG _PTReplay_PTPause(struct PTReplayIFace *Self,
	struct Module *module)
{
	dbug(("ptreplay::PTPause\n"));

	_PTSendCommand(module, PTCMD_PAUSE, 0);
	return TRUE;
}
