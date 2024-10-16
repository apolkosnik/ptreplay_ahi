#include "ptreplay_private.h"

void _PTReplay_PTFreeMod(struct PTReplayIFace *Self,
	struct Module *module)
{
	dbug(("ptreplay::PTFreeMod\n"));

	_PTFreeModInternal(module);
}
