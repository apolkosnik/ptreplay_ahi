#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

struct Module *_PTReplay_PTLoadModule(struct PTReplayIFace *, STRPTR name);
VOID _PTReplay_PTUnloadModule(struct PTReplayIFace *, struct Module * module);
ULONG _PTReplay_PTPlay(struct PTReplayIFace *, struct Module * module);
ULONG _PTReplay_PTStop(struct PTReplayIFace *, struct Module * module);
ULONG _PTReplay_PTPause(struct PTReplayIFace *, struct Module * module);
ULONG _PTReplay_PTResume(struct PTReplayIFace *, struct Module * module);
VOID _PTReplay_PTFade(struct PTReplayIFace *, struct Module * module, UBYTE speed);
VOID _PTReplay_PTSetVolume(struct PTReplayIFace *, struct Module * module, UBYTE vol);
UBYTE _PTReplay_PTSongPos(struct PTReplayIFace *, struct Module * module);
UBYTE _PTReplay_PTSongLen(struct PTReplayIFace *, struct Module * module);
UBYTE _PTReplay_PTSongPattern(struct PTReplayIFace *, struct Module * module, UWORD Pos);
UBYTE _PTReplay_PTPatternPos(struct PTReplayIFace *, struct Module * Module);
APTR _PTReplay_PTPatternData(struct PTReplayIFace *, struct Module * Module, UBYTE Pattern, UBYTE Row);
void _PTReplay_PTInstallBits(struct PTReplayIFace *, struct Module * Module, BYTE Restart, BYTE NextPattern, BYTE NextRow, BYTE Fade);
struct Module *_PTReplay_PTSetupMod(struct PTReplayIFace *, APTR ModuleFile);
void _PTReplay_PTFreeMod(struct PTReplayIFace *, struct Module * Module);
void _PTReplay_PTStartFade(struct PTReplayIFace *, struct Module * Module, UBYTE speed);
void _PTReplay_PTOnChannel(struct PTReplayIFace *, struct Module * Module, BYTE Channels);
void _PTReplay_PTOffChannel(struct PTReplayIFace *, struct Module * Module, BYTE Channels);
void _PTReplay_PTSetPos(struct PTReplayIFace *, struct Module * Module, UBYTE Pos);
void _PTReplay_PTSetPri(struct PTReplayIFace *, BYTE Pri);
BYTE _PTReplay_PTGetPri(struct PTReplayIFace *);
BYTE _PTReplay_PTGetChan(struct PTReplayIFace *);
struct PTSample *_PTReplay_PTGetSample(struct PTReplayIFace *, struct Module * Module, WORD Nr);

STATIC CONST APTR main_vectors[] =
{
    _manager_Obtain,
    _manager_Release,
    NULL,
    NULL,
    _PTReplay_PTLoadModule,
    _PTReplay_PTUnloadModule,
    _PTReplay_PTPlay,
    _PTReplay_PTStop,
    _PTReplay_PTPause,
    _PTReplay_PTResume,
    _PTReplay_PTFade,
    _PTReplay_PTSetVolume,
    _PTReplay_PTSongPos,
    _PTReplay_PTSongLen,
    _PTReplay_PTSongPattern,
    _PTReplay_PTPatternPos,
    _PTReplay_PTPatternData,
    _PTReplay_PTInstallBits,
    _PTReplay_PTSetupMod,
    _PTReplay_PTFreeMod,
    _PTReplay_PTStartFade,
    _PTReplay_PTOnChannel,
    _PTReplay_PTOffChannel,
    _PTReplay_PTSetPos,
    _PTReplay_PTSetPri,
    _PTReplay_PTGetPri,
    _PTReplay_PTGetChan,
    _PTReplay_PTGetSample,
    (APTR)-1
};
