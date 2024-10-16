#include "ptreplay_private.h"
#include "qport.h"

void player_main () {
	struct Process *thisproc;
	struct Module *module;
	struct PTReplayMessage *msg;
	struct MsgPort ahi_mp;
	struct AHIRequest req1, req2;
	struct AHIRequest *io = &req1;
	struct AHIRequest *io2 = &req2;
	struct AHIRequest *join = NULL;
	BYTE pbuf1[PLAYER_BUF_SIZE], pbuf2[PLAYER_BUF_SIZE];
	WORD *buf = (WORD *)&pbuf1, *buf2 = (WORD *)&pbuf2, *tmp;
	LONG playing = FALSE;
	LONG quit = FALSE;
	struct TagItem tags[2];

	thisproc = (APTR)SysBase->ThisTask;

	WaitPort(&thisproc->pr_MsgPort);
	msg = (struct PTReplayMessage *)GetMsg(&thisproc->pr_MsgPort);

	module = msg->ptm_Module;

	CreateQPort(&module->player_port);
	CreateQPort(&ahi_mp);

	io->ahir_Std.io_Message.mn_Node.ln_Pri  = 0;
	io->ahir_Std.io_Message.mn_Node.ln_Type = NT_REPLYMSG;
	io->ahir_Std.io_Message.mn_ReplyPort = &ahi_mp;
	io->ahir_Std.io_Message.mn_Length = sizeof(*io);
	io->ahir_Version = 2;
	if (OpenDevice("ahi.device", 0, (struct IORequest *)io, 0)) {
		io->ahir_Std.io_Device = NULL;
		goto out;
	}

	CopyMem(io, io2, sizeof(*io2));

	tags[0].ti_Tag = PTPLAY_Flags;
	tags[0].ti_Data = module->ptplay_flags;
	tags[1].ti_Tag = TAG_END;

	ReplyMsg(&msg->ptm_Msg);

	while (!quit) {
		struct PTReplayMessage *msg;

		if (!playing) {
			Wait(1 << module->player_port.mp_SigBit);
		} else {
			ULONG buf_len = PLAYER_BUF_SIZE;
			ULONG old_pos = 0;
			ULONG new_pos = 0;
			ULONG old_patpos = 0;
			ULONG new_patpos = 0;
			ULONG flags = 0;
			
			ObtainSemaphore(&module->mutex);
			tags[0].ti_Data = module->ptplay_flags;
			PtSetAttrs(module->mod_handle, tags);
			PtGetAttr(module->mod_handle, PTPLAY_SongPosition, &old_pos);
			PtGetAttr(module->mod_handle, PTPLAY_PatternPosition, &old_patpos);
			PtRender(module->mod_handle, (APTR)&buf[0], (APTR)&buf[1],
				4, 44100/10, 1, 16, 2);
			PtGetAttr(module->mod_handle, PTPLAY_SongPosition, &new_pos);
			PtGetAttr(module->mod_handle, PTPLAY_PatternPosition, &new_patpos);
			PtGetAttr(module->mod_handle, PTPLAY_Flags, &flags);
			if (new_pos != old_pos && module->signals[PTSIG_POS] != -1) {
				Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_POS]);
			}
			if ((new_pos != old_pos || new_patpos != old_patpos) &&
				module->signals[PTSIG_ROW] != -1)
			{
				Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_ROW]);
			}
			if ((flags & MODF_SONGEND) && module->signals[PTSIG_RESTART] != -1) {
				int32 *ptr = (int32 *)((int8 *)buf + buf_len);
				Signal(&module->parent_proc->pr_Task,
					1 << module->signals[PTSIG_RESTART]);
				while (buf_len > 0 && *--ptr == 0) buf_len -= 4;
				PtSeek(module->mod_handle, 0);
			}
			ReleaseSemaphore(&module->mutex);

			io->ahir_Std.io_Command = CMD_WRITE;
			io->ahir_Std.io_Data = buf;
			io->ahir_Std.io_Length = buf_len;
			io->ahir_Std.io_Offset = 0;
			io->ahir_Frequency = 44100;
			io->ahir_Type = AHIST_S16S;
			io->ahir_Volume = 0x10000;
			io->ahir_Position = 0x8000;
			io->ahir_Link = join;
			SendIO((struct IORequest *)io);
			if (join) WaitIO((struct IORequest *)join);
			join = io; io = io2; io2 = join;
			tmp = buf; buf = buf2; buf2 = tmp;
		}
		while ((msg = (struct PTReplayMessage *)GetMsg(&module->player_port))) {
			switch (msg->ptm_Command) {
				case PTCMD_PLAY:
					ObtainSemaphore(&module->mutex);
					PtSeek(module->mod_handle, 0);
					ReleaseSemaphore(&module->mutex);
					/* fall through */
				case PTCMD_RESUME:
					playing = TRUE;
					break;
				case PTCMD_STOP:
					ObtainSemaphore(&module->mutex);
					PtSeek(module->mod_handle, 0);
					ReleaseSemaphore(&module->mutex);
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
						LONG command = msg->ptm_Command;
						LONG fade_len = msg->ptm_Args[0]*(44100*64/50);
						LONG fade_pos;
						LONG buf_len = 44100/10;
						LONG buf_pos;
						WORD *ptr;
						
						if (command == PTCMD_STARTFADE) {
							ReplyMsg(&msg->ptm_Msg);
							msg = NULL;
						}
						
						ObtainSemaphore(&module->mutex);
						tags[0].ti_Data = module->ptplay_flags & ~MODF_DOSONGEND;
						PtSetAttrs(module->mod_handle, tags);
						fade_pos = fade_len;
						while (fade_pos > 0) {
							if (fade_pos < buf_len) buf_len = fade_pos;
							
							PtRender(module->mod_handle, (APTR)&buf[0], (APTR)&buf[1],
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
							SendIO((struct IORequest *)io);
							if (join) WaitIO((struct IORequest *)join);
							join = io; io = io2; io2 = join;
							tmp = buf; buf = buf2; buf2 = tmp;
						}
						ReleaseSemaphore(&module->mutex);
						
						if (command == PTCMD_STARTFADE &&
							module->signals[PTSIG_FADE] != -1)
						{
							Signal(&module->parent_proc->pr_Task,
								1 << module->signals[PTSIG_FADE]);
						}
						
						playing = FALSE;
					}
					break;
			}
			if (msg) {
				ReplyMsg(&msg->ptm_Msg);
			}
		}
	}
	
out:
	module->player_proc = NULL;

	#if 0
	if ((msg = (struct PTReplayMessage *)GetMsg(&module->player_port)))
	{
		ReplyMsg(&msg->ptm_Msg);
	}
	#endif

	if (io->ahir_Std.io_Device) {
		if (join) {
			AbortIO((struct IORequest *)join);
			WaitIO((struct IORequest *)join);
		}
		CloseDevice((struct IORequest *)io);
	}

	/* No need to explicitly free signals on exit */

	Forbid();
	ReplyMsg(&msg->death_msg);
}
