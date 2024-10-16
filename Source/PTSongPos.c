#include "ptreplay_private.h"

UBYTE _PTReplay_PTSongPos(struct PTReplayIFace *Self,
	struct Module *module)
{
	uint32 res = 0;
	
	dbug(("ptreplay::PTSongPos\n"));
	
	ObtainSemaphore(&module->mutex);
	PtGetAttr(module->mod_handle, PTPLAY_SongPosition, &res);
	ReleaseSemaphore(&module->mutex);
	
	return res;
}
