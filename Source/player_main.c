#include "ptreplay_private.h"

int player_main () {
	struct Module *module;
	struct PTReplayMessage *msg;
	struct MsgPort *main_mp;
	struct MsgPort *ahi_mp = NULL;
	struct AHIRequest *io = NULL;
	struct AHIRequest *io2 = NULL;
	struct AHIRequest *join = NULL;
	int16 *buf = NULL, *buf2 = NULL, *tmp;
	int32 playing = FALSE;
	int32 quit = FALSE;
	struct TagItem tags[2];
	
	main_mp = IDOS->GetProcMsgPort(NULL);
	IExec->WaitPort(main_mp);
	msg = (struct PTReplayMessage *)IExec->GetMsg(main_mp);
	
	module = msg->ptm_Module;
	main_mp = module->player_port;
	main_mp->mp_SigBit = IExec->AllocSignal(-1);
	main_mp->mp_SigTask = IExec->FindTask(NULL);
	main_mp->mp_Flags = PA_SIGNAL;

	ahi_mp = IExec->AllocSysObject(ASOT_PORT, NULL);
	io = IExec->AllocSysObjectTags(ASOT_IOREQUEST,
		ASOIOR_Size,		sizeof(struct AHIRequest),
		ASOIOR_ReplyPort,	ahi_mp,
		TAG_END);
	if (!io) goto out;
	io->ahir_Version = 4;
	if (IExec->OpenDevice("ahi.device", 0, (struct IORequest *)io, 0)) {
		io->ahir_Std.io_Device = NULL;
		goto out;
	}
	io2 = IExec->AllocSysObjectTags(ASOT_IOREQUEST,
		ASOIOR_Duplicate,	io,
		TAG_END);
	if (!io2) goto out;
	buf = IExec->AllocVec(44100/10*4, MEMF_SHARED);
	buf2 = IExec->AllocVec(44100/10*4, MEMF_SHARED);
	if (!buf || !buf2) goto out;

	tags[0].ti_Tag = PTPLAY_Flags;
	tags[0].ti_Data = module->ptplay_flags;
	tags[1].ti_Tag = TAG_END;

	IExec->ReplyMsg(&msg->ptm_Msg);

	while (!quit) {
		if (!playing) {
			IExec->Wait(1 << main_mp->mp_SigBit);
		} else {
			uint32 buf_len = 44100/10*4;
			uint32 old_pos = 0;
			uint32 new_pos = 0;
			uint32 old_patpos = 0;
			uint32 new_patpos = 0;
			uint32 flags = 0;
			
			IExec->MutexObtain(module->mutex);
			tags[0].ti_Data = module->ptplay_flags;
			IPtPlay->PtSetAttrs(module->mod_handle, tags);
			IPtPlay->PtGetAttr(module->mod_handle, PTPLAY_SongPosition, &old_pos);
			IPtPlay->PtGetAttr(module->mod_handle, PTPLAY_PatternPosition, &old_patpos);
			IPtPlay->PtRender(module->mod_handle, (APTR)&buf[0], (APTR)&buf[1],
				4, 44100/10, 1, 16, 2);
			IPtPlay->PtGetAttr(module->mod_handle, PTPLAY_SongPosition, &new_pos);
			IPtPlay->PtGetAttr(module->mod_handle, PTPLAY_PatternPosition, &new_patpos);
			IPtPlay->PtGetAttr(module->mod_handle, PTPLAY_Flags, &flags);
			if (new_pos != old_pos && module->signals[PTSIG_POS] != -1) {
				IExec->Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_POS]);
			}
			if ((new_pos != old_pos || new_patpos != old_patpos) &&
				module->signals[PTSIG_ROW] != -1)
			{
				IExec->Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_ROW]);
			}
			if ((flags & MODF_SONGEND) && module->signals[PTSIG_RESTART] != -1) {
				int32 *ptr = (int32 *)((int8 *)buf + buf_len);
				IExec->Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_RESTART]);
				while (buf_len > 0 && *--ptr == 0) buf_len -= 4;
				IPtPlay->PtSeek(module->mod_handle, 0);
			}
			IExec->MutexRelease(module->mutex);

			io->ahir_Std.io_Message.mn_Node.ln_Pri = 0;
			io->ahir_Std.io_Command = CMD_WRITE;
			io->ahir_Std.io_Data = buf;
			io->ahir_Std.io_Length = buf_len;
			io->ahir_Std.io_Offset = 0;
			io->ahir_Frequency = 44100;
			io->ahir_Type = AHIST_S16S;
			io->ahir_Volume = 0x10000;
			io->ahir_Position = 0x8000;
			io->ahir_Link = join;
			IExec->SendIO((struct IORequest *)io);
			if (join) IExec->WaitIO((struct IORequest *)join);
			join = io; io = io2; io2 = join;
			tmp = buf; buf = buf2; buf2 = tmp;
		}
		while ((msg = (struct PTReplayMessage *)IExec->GetMsg(main_mp))) {
			switch (msg->ptm_Command) {
				case PTCMD_PLAY:
					IExec->MutexObtain(module->mutex);
					IPtPlay->PtSeek(module->mod_handle, 0);
					IExec->MutexRelease(module->mutex);
					/* fall through */
				case PTCMD_RESUME:
					playing = TRUE;
					break;
				case PTCMD_STOP:
					IExec->MutexObtain(module->mutex);
					IPtPlay->PtSeek(module->mod_handle, 0);
					IExec->MutexRelease(module->mutex);
					/* fall through */
				case PTCMD_PAUSE:
					playing = FALSE;
					break;
				case PTCMD_DIE:
					msg = NULL;
					quit = TRUE;
					break;
				case PTCMD_FADE:
				case PTCMD_STARTFADE:
					if (playing) {
						int32 command = msg->ptm_Command;
						int32 fade_len = msg->ptm_Args[0]*(44100*64/50);
						int32 fade_pos;
						int32 buf_len = 44100/10;
						int32 buf_pos;
						int16 *ptr;
						
						if (command == PTCMD_STARTFADE) {
							IExec->ReplyMsg(&msg->ptm_Msg);
							msg = NULL;
						}
						
						IExec->MutexObtain(module->mutex);
						tags[0].ti_Data = module->ptplay_flags & ~MODF_DOSONGEND;
						IPtPlay->PtSetAttrs(module->mod_handle, tags);
						fade_pos = fade_len;
						while (fade_pos > 0) {
							if (fade_pos < buf_len) buf_len = fade_pos;
							
							IPtPlay->PtRender(module->mod_handle, (APTR)&buf[0], (APTR)&buf[1],
								4, buf_len, 1, 16, 2);
							
							ptr = buf;
							for (buf_pos = 0; buf_pos < buf_len; buf_pos++, fade_pos--) {
								ptr[0] = (int32)ptr[0] * fade_pos / fade_len;
								ptr[1] = (int32)ptr[1] * fade_pos / fade_len;
								ptr += 2;
							}
							
							io->ahir_Std.io_Message.mn_Node.ln_Pri = 0;
							io->ahir_Std.io_Command = CMD_WRITE;
							io->ahir_Std.io_Data = buf;
							io->ahir_Std.io_Length = buf_len << 2;
							io->ahir_Std.io_Offset = 0;
							io->ahir_Frequency = 44100;
							io->ahir_Type = AHIST_S16S;
							io->ahir_Volume = 0x10000;
							io->ahir_Position = 0x8000;
							io->ahir_Link = join;
							IExec->SendIO((struct IORequest *)io);
							if (join) IExec->WaitIO((struct IORequest *)join);
							join = io; io = io2; io2 = join;
							tmp = buf; buf = buf2; buf2 = tmp;
						}
						IExec->MutexRelease(module->mutex);
						
						if (command == PTCMD_STARTFADE &&
							module->signals[PTSIG_FADE] != -1)
						{
							IExec->Signal(&module->parent_proc->pr_Task,
								1 << module->signals[PTSIG_FADE]);
						}
						
						playing = FALSE;
					}
					break;
			}
			if (msg) {
				IExec->ReplyMsg(&msg->ptm_Msg);
			}
		}
	}
	
out:
	if (io && io->ahir_Std.io_Device) {
		if (join) {
			IExec->AbortIO((struct IORequest *)join);
			IExec->WaitIO((struct IORequest *)join);
		}
		IExec->CloseDevice((struct IORequest *)io);
	}
	IExec->FreeVec(buf);
	IExec->FreeVec(buf2);
	IExec->FreeSysObject(ASOT_IOREQUEST, io);
	IExec->FreeSysObject(ASOT_IOREQUEST, io2);
	IExec->FreeSysObject(ASOT_PORT, ahi_mp);

	main_mp->mp_Flags = PA_IGNORE;
	IExec->FreeSignal(main_mp->mp_SigBit);
	main_mp->mp_SigTask = NULL;
	main_mp->mp_SigBit = -1;
	
	module->player_proc = NULL;
	
	return 0;
}
