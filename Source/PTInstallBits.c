#include "ptreplay_private.h"

void _PTReplay_PTInstallBits(struct PTReplayIFace *Self,
	struct Module * module, BYTE Restart, BYTE NextPattern,
	BYTE NextRow, BYTE Fade)
{
	dbug(("ptreplay::PTInstallBits\n"));
	
	module->signals[PTSIG_RESTART] = Restart;
	module->signals[PTSIG_POS] = NextPattern;
	module->signals[PTSIG_ROW] = NextRow;
	module->signals[PTSIG_FADE] = Fade;
	if (Restart != -1) {
		module->ptplay_flags |= MODF_DOSONGEND;
	} else {
		module->ptplay_flags &= ~MODF_DOSONGEND;
	}
}
