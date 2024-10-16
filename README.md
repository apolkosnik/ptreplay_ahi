# ptreplay_ahi
ProTracker player library


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
