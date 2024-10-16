#ifndef PTREPLAY_INTERFACE_DEF_H
#define PTREPLAY_INTERFACE_DEF_H
/*
** This file is machine generated from idltool
** Do not edit
*/ 

#include <exec/types.i>
#include <exec/exec.i>
#include <exec/interfaces.i>

STRUCTURE PTReplayIFace, InterfaceData_SIZE
	    FPTR IPTReplay_Obtain
	    FPTR IPTReplay_Release
	    FPTR IPTReplay_Expunge
	    FPTR IPTReplay_Clone
	    FPTR IPTReplay_PTLoadModule
	    FPTR IPTReplay_PTUnloadModule
	    FPTR IPTReplay_PTPlay
	    FPTR IPTReplay_PTStop
	    FPTR IPTReplay_PTPause
	    FPTR IPTReplay_PTResume
	    FPTR IPTReplay_PTFade
	    FPTR IPTReplay_PTSetVolume
	    FPTR IPTReplay_PTSongPos
	    FPTR IPTReplay_PTSongLen
	    FPTR IPTReplay_PTSongPattern
	    FPTR IPTReplay_PTPatternPos
	    FPTR IPTReplay_PTPatternData
	    FPTR IPTReplay_PTInstallBits
	    FPTR IPTReplay_PTSetupMod
	    FPTR IPTReplay_PTFreeMod
	    FPTR IPTReplay_PTStartFade
	    FPTR IPTReplay_PTOnChannel
	    FPTR IPTReplay_PTOffChannel
	    FPTR IPTReplay_PTSetPos
	    FPTR IPTReplay_PTSetPri
	    FPTR IPTReplay_PTGetPri
	    FPTR IPTReplay_PTGetChan
	    FPTR IPTReplay_PTGetSample
	LABEL PTReplayIFace_SIZE

#endif
