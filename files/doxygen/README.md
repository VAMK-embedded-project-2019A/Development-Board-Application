Front Page
========================

Development Board Software
========================
Part of the Smart Music Player project. Program for the development board.  
[Github project](https://github.com/VAMK-embedded-project-2019A/Development-Board-Application).

System requirements
------------------------
* __Operating system__: Raspbian Buster with desktop and recommended software (September 2019)  
* __Compiler__: g++ 8.3.0  
* __Libraries__  

Name | Version
--- | ---
libssl-dev | 1.1.1c-1
libcurl4-openssl-dev | 7.64.0-4
libmpg123-dev | 1.25.10-2
libjsoncpp-dev | 1.7.4-3

Install dependencies
------------------------
```console
[pi@raspberry:~]$ sudo apt install libssl-dev=1.1.1c-1  
[pi@raspberry:~]$ sudo apt install libjsoncpp-dev=1.7.4-3  
[pi@raspberry:~]$ sudo apt install libcurl4-openssl-dev=7.64.0-4  
[pi@raspberry:~]$ sudo apt install libmpg123-dev=1.25.10-2
```

Configurations
------------------------
__NOTE__: Keep the passpharse and private key secret. The folder ./files/config-files/ has been ignored by git.  
* Generate an private/public key pair for the SFTP communication  
```console
[pi@raspberry:~]$ sudo ssh-keygen -m PEM -t rsa -N "" -f ./files/config-files/ssh-key
```
* Upload the public key to the server so that it will trust us for accessing the specified user's account
```console
[pi@raspberry:~]$ sudo ssh-copy-id -i ./files/config-files/ssh-key.pub <server_username>@<server_IP>
```
* Save the server as known host
```console
[pi@raspberry:~]$ ssh-keyscan <server_IP> > ./files/config-files/known-hosts
```
* If any path change, save the configurations to the configuration file
```console
[pi@raspberry:~]$ nano ./files/config.txt
```

Build and run
------------------------
```console
[pi@raspberry:~]$ make -j4
[pi@raspberry:~]$ chmod +x run.sh
[pi@raspberry:~]$ ./run.sh
```

Build test
------------------------
```console
[pi@raspberry:~]$ make test -j4
[pi@raspberry:~]$ LD_LIBRARY_PATH=./test ./test/test
```


Autostart
------------------------
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