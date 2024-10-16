#include "ptreplay_private.h"

UBYTE _PTReplay_PTSongLen(struct PTReplayIFace *Self,
	struct Module *module)
{
	uint32 res = 0;
	
	dbug(("ptreplay::PTSongLen\n"));
	
	ObtainSemaphore(&module->mutex);
	PtGetAttr(module->mod_handle, PTPLAY_Positions, &res);
	ReleaseSemaphore(&module->mutex);
	
	return res;
}
