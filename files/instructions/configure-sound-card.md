# Configure sound card
Load the sound driver and check if it is loaded:
```console
[pi@raspberry:~]$ sudo modprobe snd-bcm2835
[pi@raspberry:~]$ sudo lsmod | grep 2835
```
Select the output device for sound (0 = default/auo, 1 = jack 3.5, 2 = HDMI)
```console
[pi@raspberry:~]$ sudo amixer cset numid=3 1
```
Open terminal and test if ALSA is working (in either the built-in 3.5mm jack or sound card's jack)
```console
[pi@raspberry:~]$ aplay /usr/share/sounds/alsa/Front_Center.wav
```
It is required to have the file **asound.conf** to set up output hardware/sound card properly.
```console
[pi@raspberry:~]$ sudo nano /etc/asound.conf
```
Copy and paste the following code and save it:
```
pcm.!default {
	type plug
	slave {
		pcm "hw:1,0"
	}
}
ctl.!default {
	type hw
	card 1
}
```