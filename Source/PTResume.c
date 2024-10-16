#include "ptreplay_private.h"

ULONG _PTReplay_PTResume(struct PTReplayIFace *Self,
	struct Module *module)
{
	dbug(("ptreplay::PTResume\n"));

	_PTSendCommand(module, PTCMD_RESUME);
	return TRUE;
}
