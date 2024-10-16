#include "ptreplay_private.h"

void _PTReplay_PTSetPos(struct PTReplayIFace *Self,
	struct Module *module, UBYTE pos)
{
	struct TagItem tags[2];

	dbug(("ptreplay::PTSetPos\n"));
	
	tags[0].ti_Tag = PTPLAY_SongPosition;
	tags[0].ti_Data = pos;
	tags[1].ti_Tag = TAG_END;
	ObtainSemaphore(&module->mutex);
	PtSetAttrs(module->mod_handle, tags);
	ReleaseSemaphore(&module->mutex);
}
