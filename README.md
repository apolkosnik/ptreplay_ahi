# ptreplay_ahi
ProTracker player library

The reason for this repo's existance is that IMP3 (https://aminet.net/package/mus/play/imp3) can be made to use of tthe retargetable audio (AHI) to play modules. The problem was that the ptplay.library as found on the Aminet would often crash when used in conjunction with PTReplayLibrary_AHI (the wrapper around PTPlayLibrary, not the PTReplayLibrary 6.6 or 7.0 which use Paula audio). I've noticed that there was a slightly updated version for OS4, but there was no backport for OS3, so...

It was taken from https://aminet.net/package/mus/play/ptreplay_ahi.lha, diffed against https://aminet.net/package/util/libs/ptreplay_lib.lha and updated with some changes. The Makefile was adjusted to cross-compile and all that is in the git history.

I've built it on Linux x86_64 with https://github.com/nicolasbauw/amiga-cc by clonning it to /opt and following the directions.


The original ptreplay_ahi.readme, as can be found on the Aminet.net:
 -------------------------------------------------------------------


This is port of ptreplay.library wrapper written by Fredrik Wikstrom.
The original ptreplay.library written by Mattias karlsson and Andreas Pålsson
bangs the hardware directly and runs only on original Amigas.
Fredrik Wikstrom's ptreplay.library wrapper cures this problem by using
ptplay.library and ahi.device for playback. This wrapper library is now ported
to OS3 and MorphOS.

 Requirements:
 -------------

 - Kickstart 2.0 or newer
 - AHI version 2 or newer
 - ptplay.library


 Installation:
 -------------

 Amiga (universal version)
  Copy Amiga/Libs/ptreplay.library LIBS:

 MorphOS (MorphOS 1 or newer)
  Copy MorphOS/Libs/ptreplay.library LIBS:


 Changes to the original OS4 version:
 ------------------------------------

 - pops up a requester if ptplay.library is not found
 - player process runs at priority +5
 - some internal changes to make it build for Kickstart 2.0/3.0


 The original version written for OS4 is available at Aminet:
   http://aminet.net/util/libs/ptreplay_lib.lha

 The most original version written by Mattias Karlsson and Andreas Pålsson
 is also available at Aminet:
   http://aminet.net/package/mus/play/ptreplay66

 It is still useful if you have the original Amiga and prefer using Paula
 for audio playback without AHI overhead.
