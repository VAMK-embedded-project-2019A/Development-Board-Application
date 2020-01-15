[![Build Status](https://travis-ci.com/VAMK-embedded-project-2019A/Development-Board-Application.svg?branch=master)](https://travis-ci.com/VAMK-embedded-project-2019A/Development-Board-Application) [![Coverage Status](https://coveralls.io/repos/github/VAMK-embedded-project-2019A/Development-Board-Application/badge.svg?branch=master)](https://coveralls.io/github/VAMK-embedded-project-2019A/Development-Board-Application?branch=master) [![Total alerts](https://img.shields.io/lgtm/alerts/g/VAMK-embedded-project-2019A/Development-Board-Application.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/VAMK-embedded-project-2019A/Development-Board-Application/alerts/) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/VAMK-embedded-project-2019A/Development-Board-Application.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/VAMK-embedded-project-2019A/Development-Board-Application/context:cpp)

# Development Board Application
Part of the Smart Music Player project. Program for the development board.

## Documentation
The documentation can be found on the [project homepage](https://vamk-embedded-project-2019a.github.io/Development-Board-Application/).

## Requirements
* Raspberry Pi 3B+ running Raspbian Buster with desktop and recommended software (September 2019)
* Microstack GPS module.
* Deltaco USB sound card 7.1

## Install dependencies
```console
[pi@raspberry:~]$ sudo apt install libssl-dev=1.1.1c-1  
[pi@raspberry:~]$ sudo apt install libjsoncpp-dev=1.7.4-3  
[pi@raspberry:~]$ sudo apt install libcurl4-openssl-dev=7.64.0-4  
[pi@raspberry:~]$ sudo apt install libmpg123-dev=1.25.10-2
```

## Configurations
__Required__
* Generate key pair for SFTP communication [instruction](https://github.com/VAMK-embedded-project-2019A/Development-Board-Application/tree/master/files/instructions/generate-key-pair.md)
* Configuration for GPS module [instruction](https://github.com/VAMK-embedded-project-2019A/Development-Board-Application/tree/master/files/instructions/)
* Configuration for USB sound card [instruction](https://github.com/VAMK-embedded-project-2019A/Development-Board-Application/tree/master/files/instructions/configure-sound-card.md)
__Not required__
* Configuration to start the program automatically after boot [instruction](https://github.com/VAMK-embedded-project-2019A/Development-Board-Application/tree/master/files/instructions/autostart.md)

## Build and run
```console
[pi@raspberry:~]$ make -j4
[pi@raspberry:~]$ chmod +x run.sh
[pi@raspberry:~]$ ./run.sh
```

## Build test
```console
[pi@raspberry:~]$ make test -j4
[pi@raspberry:~]$ LD_LIBRARY_PATH=./test ./test/test
```