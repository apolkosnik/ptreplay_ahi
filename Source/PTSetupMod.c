#include "ptreplay_private.h"

struct Module * _PTReplay_PTSetupMod(struct PTReplayIFace *Self,
	APTR ModuleFile)
{
	dbug(("ptreplay::PTSetupMod\n"));

	if (ModuleFile)
	{
		struct Module *module;

		module = AllocMem(sizeof(*module), MEMF_ANY);

		if (module)
		{
			module->file_data = ModuleFile;
			module->file_size = 0x10000000;
			module->free_file_data = FALSE;

			return _PTSetupModInternal(module, NULL);
		}
	}

	return NULL;
}
