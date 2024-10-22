#ifndef CLIB_PTREPLAY_PROTOS_H
#define CLIB_PTREPLAY_PROTOS_H

APTR PTLoadModule(CONST_STRPTR name);
VOID PTUnloadModule(APTR module);
ULONG PTPlay(APTR module);
ULONG PTStop(APTR module);
ULONG PTPause(APTR module);
ULONG PTResume(APTR module);
VOID PTFade(APTR module, UBYTE speed);
VOID PTSetVolume(APTR module, UBYTE vol);
UBYTE PTSongPos(APTR module);
UBYTE PTSongLen(APTR module);
UBYTE PTSongPattern(APTR module, UWORD Pos);
UBYTE PTPatternPos(APTR Module);
APTR PTPatternData(APTR Module, UBYTE Pattern, UBYTE Row);
void PTInstallBits(APTR Module, BYTE Restart, BYTE NextPattern, BYTE NextRow, BYTE Fade);
APTR PTSetupMod(APTR ModuleFile);
void PTFreeMod(APTR Module);
void PTStartFade(APTR Module, UBYTE speed);
void PTOnChannel(APTR Module, BYTE Channels);
void PTOffChannel(APTR Module, BYTE Channels);
void PTSetPos(APTR Module, UBYTE Pos);
void PTSetPri(BYTE Pri);
BYTE PTGetPri();
BYTE PTGetChan();
struct PTSample * PTGetSample(APTR Module, WORD Nr);

#endif /* CLIB_PTREPLAY_PROTOS_H */
