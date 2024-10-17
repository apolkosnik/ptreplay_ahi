/*
 *  $Id: $
 */

#include <exec/execbase.h>
#include <proto/exec.h>
#include <exec/lists.h>

void CreateQPort(struct MsgPort *port)
{
	port->mp_Node.ln_Type = NT_MSGPORT;
	port->mp_Flags        = PA_SIGNAL;
	if ((BYTE) (port->mp_SigBit = AllocSignal(-1)) == -1)
	{
		port->mp_SigBit = SIGB_SINGLE;
		SetSignal(0, SIGF_SINGLE);
	}
	port->mp_SigTask      = SysBase->ThisTask;
	NewList(&port->mp_MsgList);
}

void DeleteQPort(struct MsgPort *port)
{
	if (port->mp_SigTask)
	{
		if (port->mp_SigBit == SIGB_SINGLE)
			SetSignal(0, SIGF_SINGLE);
		else
			FreeSignal(port->mp_SigBit);
	}
}
