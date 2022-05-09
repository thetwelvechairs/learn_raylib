[![Build](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml/badge.svg)](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=twelvechairssoftware_learn_raylib&metric=alert_status)](https://sonarcloud.io/dashboard?id=twelvechairssoftware_learn_raylib)

Raspberry Pi / TFT screen project for drawing graphics on a Raspberry Pi without the need for X or other windowing environments.



### TFT support
````bash
sudo curl https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/pitft-fbcp.sh >pitft-fbcp.sh
sudo bash pitft-fbcp.sh
````

### Raspberry Pi (buster) dependencies
````bash
sudo apt-get install -y build-essential git cmake python3-pip curl libxrandr-dev libxinerama-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev libegl-dev
pip3 install conan
export PATH="$HOME/.local/bin:$PATH"
````

### Build
````bash
# Not strictly necessary, but I develop on a Mac
killall learn_raylib

git fetch
git reset --hard HEAD
git pull

sed -i 's/..\/src\/include\/unicorn_body.obj/src\/include\/unicorn_body.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_horn.obj/src\/include\/unicorn_horn.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_mane.obj/src\/include\/unicorn_mane.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_tail.obj/src\/include\/unicorn_tail.obj/g' src/main.cpp

export CONAN_SYSREQUIRES_MODE=enabled
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target learn_raylib --clean-first -- -j 4

./learn_raylib
````
