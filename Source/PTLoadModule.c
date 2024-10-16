#include "ptreplay_private.h"

STATIC APTR load_file (const char *filename, int32 *file_size) {
	BPTR file;
	APTR memory = NULL;
	file = Open(filename, MODE_OLDFILE);
	if (file)
	{
		D_S(struct FileInfoBlock, fib);

		if (ExamineFH(file, fib))
		{
			*file_size = fib->fib_Size;

			if (fib->fib_Size > 1084)		// 'M.K.' is at offset 1080 but of course min size is much more than that
			{
				memory = AllocMem(*file_size, MEMF_ANY);
				if (memory && Read(file, memory, *file_size) != *file_size) {
					FreeMem(memory, *file_size);
					memory = NULL;
				}
			}
		}
		Close(file);
	}
	return memory;
}

struct Module *_PTReplay_PTLoadModule(struct PTReplayIFace *Self,
	STRPTR name)
{
	struct Module *module;
	
	//IExec->DebugPrintF("ptreplay::PTLoadModule\n");
	
	module = AllocMem(sizeof(*module), MEMF_ANY);

	if (module)
	{
		module->file_data = load_file(name, &module->file_size);
		module->free_file_data = TRUE;
	
		return _PTSetupModInternal(module, name);
	}

	return NULL;
}
