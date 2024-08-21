
https://github.com/user-attachments/assets/c8c24d14-7a68-4b7e-8557-1de7d7fc061a

multiband compressor made with modern c++ using JUCE framework

**features:**

 -input/output trim controls

 -audio spectrum analyzer 

 -low-mid and mid-high crossovers to select band
 
 -bypass/solo/mute buttons for each band
 
 -attack, release, threshold, and ratio compressor controls for each band

**additional notes:**

Why is a compressor necessary? The signal's amplitude can't be greater than what the system can support. If the signal is too much for the soundcard's limits, it will take that signal and clamp it straight (a brick walled limit) and will no longer be accurate to the original signal.

A compressor THRESHOLD will clamp any signal above the threshold to stay near or below the threshold.

A compressor RATIO will control how much gain is applied to the signal. (X:1)

A compressor ATTACK controls how long to wait after signal passes threshhold before gain reduction is applied and RELEASE controls how long gain reduction stays after signal leaves threshold.

Compression can be very subtle, even more so when applied to multiple small ranges of frequencies like this. That being said, it may be difficult hearing the effects of an audio compressor, especially if it's your first time. It  takes audio enginereers a while to develop this kind ofauditory sensitivity.
