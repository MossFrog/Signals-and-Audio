# Signals-and-Audio
A small program created in C++ for the Signals course at the University of Maribor.

The program accepts any OGG/WAV file with a sampling frequency of 44100Hz and then creates multiple outputs with lower bit depths and sampling frequencies.

The program also accepts adding effects to a given audio sample by selecting an impulse response file. A nice compilation can be found under "IM Reverbs Pack" at this link http://www.voxengo.com/impulses/

Supports recording audio directly from a microphone.

The program also renders the waveform statically in multiple colors.

An additional High-Pass whistle filter has been added as an example.

Controls

- Hold 'R' to record audio.
- Press 'P' to playback audio and update the waveform rendering.
- Press 'C' to change the Waveform color.

The resources used:
- SFML 2.3.2.
- Native File Dialog Library by Michael Labbe.
- Strutils classes.
- LIBROW FFT and Complex classes.

Special thanks to:


The University of Duke for their String Utilities classes.


Michael Labbe for his Native File Dialog C Libraries.

