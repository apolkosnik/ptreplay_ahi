#include "ptreplay_private.h"

UBYTE _PTReplay_PTSongPattern(struct PTReplayIFace *Self,
	struct Module *module, UWORD Pos)
{
	dbug(("ptreplay::PTSongPattern\n"));

    UBYTE* p = (UBYTE *) module->file_data;
    if (Pos <= 128) {
        return p[952 + Pos];
    }
	return (UBYTE)0;
}
