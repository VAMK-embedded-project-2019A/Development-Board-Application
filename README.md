[![Build Status](https://travis-ci.com/VAMK-embedded-project-2019A/Device-Software.svg?branch=master)](https://travis-ci.com/VAMK-embedded-project-2019A/Device-Software) [![Coverage Status](https://coveralls.io/repos/github/VAMK-embedded-project-2019A/Device-Software/badge.svg)](https://coveralls.io/github/VAMK-embedded-project-2019A/Device-Software)

# Device Software
Part of the Smart Music Player project. Main program for the UDOO board.

## System requirements
* __Operating system__: UDOObuntu 2.3.0 (Ubuntu 14.04 LTS)  
* __Compiler__: g++ 4.8.4  
* __Libraries__  

Name | Version
--- | ---
libssl-dev | 0.0.0
libcurl-dev | 0.0.0
libbluetooth-dev | 0.0.0
libjsoncpp-dev | 0.0.0

## Configurations
__NOTE__: Keep the passpharse and private key secret  
* Generate an private/public key pair (protected with passphrase) for the SFTP communication  
```console
[udooer@udoo:~]$ sudo ssh-keygen -t rsa -f ./files/config-files/ssh-key
```
* Upload the public key to the server so that it will trust us for accessing the specified user's account
```console
[udooer@udoo:~]$ ssh-copy-id -i ./files/config-files/ssh-key.pub <server_username>@<server_IP>
```
* Save the server as known host
```console
[udooer@udoo:~]$ ssh-keyscan <server_IP> > ./files/config-files/known-hosts
```
* Save passphrase
```console
[udooer@udoo:~]$ nano ./files/config-files/passphrase
```
* If any path change, save the configurations to the configuration file
```console
[udooer@udoo:~]$ nano ./files/config.txt
```

## Build and run
```console
[udooer@udoo:~]$ make -j4
[udooer@udoo:~]$ chmod +x run.sh
[udooer@udoo:~]$ ./run.sh
```

## Build test
```console
[udooer@udoo:~]$ make test -j4
[udooer@udoo:~]$ LD_LIBRARY_PATH=./test ./test/test
```
