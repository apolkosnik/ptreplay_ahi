#include "ptreplay_private.h"

VOID _PTReplay_PTSetVolume(struct PTReplayIFace *Self,
	struct Module *module, UBYTE vol)
{
	struct TagItem tags[2];

	dbug(("ptreplay::PTSetVolume\n"));
	
	tags[0].ti_Tag = PTPLAY_MasterVolume;
	tags[0].ti_Data = (uint32)vol << 2;
	tags[1].ti_Tag = TAG_END;
	ObtainSemaphore(&module->mutex);
	PtSetAttrs(module->mod_handle, tags);
	ReleaseSemaphore(&module->mutex);
}
