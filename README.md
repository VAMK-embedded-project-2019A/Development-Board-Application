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
```console
[udooer@udoo:~]$ cat ./files/config.txt
[udooer@udoo:~]$ ssh-keyscan <Project-server_IP> > ./files/config-files/known-hosts
```

## How to use
```console
[udooer@udoo:~]$ make -j4
[udooer@udoo:~]$ chmod +x run.sh
[udooer@udoo:~]$ ./run.sh
```

## How to test
```console
[udooer@udoo:~]$ make test -j4
[udooer@udoo:~]$ LD_LIBRARY_PATH=./test ./test/test
```
