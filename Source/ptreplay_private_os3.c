#include <stdarg.h>

#include <dos/dostags.h>

#include "ptreplay_private.h"
#include "qport.h"

struct Module *_PTSetupModInternal(struct Module *module, STRPTR name)
{
	module->player_proc = NULL;
	module->reply_port.mp_SigTask = NULL;

	if (module->file_data)
	{
		ULONG i;

		for (i = 0; i < PTSIG_MAX; i++) {
			module->signals[i] = -1;
		}
		module->ptplay_flags = MODF_ALLOWFILTER;

		module->mod_type = PtTest(name, module->file_data, module->file_size);
		module->mod_handle = PtInit(module->file_data, module->file_size, 44100, module->mod_type);

		if (module->mod_handle)
		{
			CreateQPort(&module->reply_port);
			InitSemaphore(&module->mutex);

			module->cmd_msg.ptm_Msg.mn_ReplyPort = &module->reply_port;
			module->cmd_msg.death_msg.mn_ReplyPort = &module->reply_port;

			module->parent_proc = (struct Process *)SysBase->ThisTask;

			#if defined(__MORPHOS__)
			module->player_proc = CreateNewProcTags(
				NP_Priority    , 5,
				NP_Name        , "ptreplay.library player process",
				NP_Entry       , player_main,
				NP_CodeType    , CODETYPE_PPC,
				NP_PPCStackSize, PLAYER_BUF_SIZE * 2 + 8192,
				//NP_StartupMsg,	&module->death_msg,
				TAG_END);
			#else
			module->player_proc = CreateNewProcTags(
				NP_Priority , 5,
				NP_Name     , "ptreplay.library player process",
				NP_Entry    , player_main,
				NP_StackSize, PLAYER_BUF_SIZE * 2 + 4096,
				TAG_END);
			#endif

			if (module->player_proc)
			{
				module->cmd_msg.ptm_Module = module;
				module->cmd_msg.ptm_Command = PTCMD_STARTUP;
				module->cmd_msg.ptm_Args = NULL;

				PutMsg(&module->player_proc->pr_MsgPort, &module->cmd_msg.ptm_Msg);
				WaitPort(&module->reply_port);

				if (GetMsg(&module->reply_port) != &module->cmd_msg.death_msg)
				{
					return module;
				}

				module->player_proc = NULL;
			}
		}
	}

	PTUnloadModule(module);
	return NULL;
}

void _PTFreeModInternal(struct Module *module)
{
	if (!module) return;

	_PTSendCommand(module, PTCMD_DIE);
	DeleteQPort(&module->reply_port);
	PtCleanup(module->mod_handle);

	if (module->free_file_data) {
		FreeMem(module->file_data, module->file_size);
	}
	FreeMem(module, sizeof(*module));
}

void _PTSendCommand(struct Module *module, ULONG command, ...)
{
	if (module && module->player_proc)
	{
		ULONG *ptr, size, i, cnt;

		switch (command)
		{
			default:
			case PTCMD_DIE:
			case PTCMD_PAUSE:
			case PTCMD_PLAY:
			case PTCMD_STOP:
				cnt = 0;
				break;

			case PTCMD_STARTFADE:
				cnt = 1;
				break;
		}

		size = cnt * sizeof(LONG);
		ptr = NULL;

		if (cnt)
			ptr = AllocMem(size, MEMF_ANY);

		if (!cnt || ptr)
		{
			if (cnt)
			{
				va_list args;
				va_start(args, command);

				for (i = 0; i < cnt; i++)
				{
					ptr[i] = va_arg(args, ULONG);
				}

				va_end(args);
			}

			module->cmd_msg.ptm_Command = command;
			module->cmd_msg.ptm_Args = ptr;

			PutMsg(&module->player_port, &module->cmd_msg.ptm_Msg);
			WaitPort(&module->reply_port);
			GetMsg(&module->reply_port);

			if (cnt)
				FreeMem(ptr, size);
		}
	}
}
