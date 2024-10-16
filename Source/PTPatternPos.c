#include "ptreplay_private.h"

UBYTE _PTReplay_PTPatternPos(struct PTReplayIFace *Self,
	struct Module *module)
{
	uint32 res = 0;
	
	dbug(("ptreplay::PTPatternPos\n"));
	
	ObtainSemaphore(&module->mutex);
	PtGetAttr(module->mod_handle, PTPLAY_PatternPosition, &res);
	ReleaseSemaphore(&module->mutex);
	
	return res;
}
