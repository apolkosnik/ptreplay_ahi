#include "ptreplay_private.h"
#include <stdarg.h>

struct Module *_PTSetupModInternal(struct PTReplayIFace *Self,
	struct Module *module, STRPTR name)
{
	int32 i;

	if (!module || !module->file_data) goto out;
	
	for (i = 0; i < PTSIG_MAX; i++) {
		module->signals[i] = -1;
	}
	module->ptplay_flags = MODF_ALLOWFILTER;

	module->mutex = IExec->AllocSysObject(ASOT_MUTEX, NULL);
	if (!module->mutex) goto out;
	
	module->mod_type = IPtPlay->PtTest(name, module->file_data,
		module->file_size);
	module->mod_handle = IPtPlay->PtInit(module->file_data, module->file_size,
		44100, module->mod_type);
	if (!module->mod_handle) goto out;
	
	module->reply_port = IExec->AllocSysObjectTags(ASOT_PORT,
		TAG_END);
	module->player_port = IExec->AllocSysObjectTags(ASOT_PORT,
		ASOPORT_AllocSig,	FALSE,
		ASOPORT_Action,		PA_IGNORE,
		TAG_END);
	if (!module->reply_port || !module->player_port) goto out;
	
	module->cmd_msg = IExec->AllocSysObjectTags(ASOT_MESSAGE,
		ASOMSG_Size,		sizeof(struct PTReplayMessage),
		ASOMSG_ReplyPort,	module->reply_port,
		TAG_END);
	module->death_msg = IExec->AllocSysObjectTags(ASOT_MESSAGE,
		ASOMSG_Size,		sizeof(struct DeathMessage),
		ASOMSG_ReplyPort,	module->reply_port,
		TAG_END);
	if (!module->cmd_msg || !module->death_msg) goto out;
	
	module->parent_proc = (struct Process *)IExec->FindTask(NULL);
	module->player_proc = IDOS->CreateNewProcTags(
		NP_Priority,				5,
		NP_Name,					"ptreplay.library player process",
		NP_Entry,					player_main,
		NP_Child,					TRUE,
		NP_StackSize,				65536,
		NP_NotifyOnDeathMessage,	module->death_msg,
		TAG_END);
	if (!module->player_proc) goto out;
	module->cmd_msg->ptm_Module = module;
	module->cmd_msg->ptm_Command = PTCMD_STARTUP;
	module->cmd_msg->ptm_Args = NULL;
	IExec->PutMsg(&module->player_proc->pr_MsgPort, &module->cmd_msg->ptm_Msg);
	IExec->WaitPort(module->reply_port);
	if (IExec->GetMsg(module->reply_port) == &module->death_msg->dm_Msg) {
		module->player_proc = NULL;
		goto out;
	}
	
	return module;
out:
	Self->PTUnloadModule(module);
	return NULL;
}

void _PTFreeModInternal(struct PTReplayIFace *Self,
	struct Module *module)
{
	if (!module) return;

	_PTSendCommand(Self, module, PTCMD_DIE);
	IExec->FreeSysObject(ASOT_MESSAGE, module->cmd_msg);
	IExec->FreeSysObject(ASOT_MESSAGE, module->death_msg);
	IExec->FreeSysObject(ASOT_PORT, module->reply_port);
	IExec->FreeSysObject(ASOT_PORT, module->player_port);
	IPtPlay->PtCleanup(module->mod_handle);
	IExec->FreeSysObject(ASOT_MUTEX, module->mutex);
	if (module->free_file_data) {
		IExec->FreeVec(module->file_data);
	}
	IExec->FreeVec(module);
}

void VARARGS68K _PTSendCommand(struct PTReplayIFace *Self,
	struct Module *module, int32 command, ...)
{
	va_list args;
	va_startlinear(args, command);
	if (module && module->player_proc) {
		module->cmd_msg->ptm_Command = command;
		module->cmd_msg->ptm_Args = va_getlinearva(args, uint32 *);
		IExec->PutMsg(module->player_port, &module->cmd_msg->ptm_Msg);
		IExec->WaitPort(module->reply_port);
		IExec->GetMsg(module->reply_port);
	}
	va_end(args);
}
