# Autostart
In the file `musicplayer.service`, fix the directory configuration if needed.  
```console
[pi@raspberry:~]$ nano ./files/musicplayer.service
```
Copy the service file to `/etc/systemd/system`
```console
[pi@raspberry:~]$ sudo cp ./files/musicplayer.service /etc/systemd/system/
```
Start the service automatically on reboot
```console
[pi@raspberry:~]$ sudo systemctl enable myscript.service
```
The service log can be checked using
```console
[pi@raspberry:~]$ journalctl -u musicplayer.service
```