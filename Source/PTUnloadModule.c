#include "ptreplay_private.h"

void _PTReplay_PTUnloadModule(struct PTReplayIFace *Self,
	struct Module *module)
{
	dbug(("ptreplay::PTUnloadModule\n"));

	_PTFreeModInternal(module);
}
