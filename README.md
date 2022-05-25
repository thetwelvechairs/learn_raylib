[![Build](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml/badge.svg)](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=twelvechairssoftware_learn_raylib&metric=alert_status)](https://sonarcloud.io/dashboard?id=twelvechairssoftware_learn_raylib)

Raspberry Pi / TFT screen project for drawing graphics on a Raspberry Pi without the need for X or other windowing environments.



### Raspberry Pi (buster) dependencies
````bash
sudo apt-get install -y build-essential git cmake python3-pip curl libx11-dev libxcursor-dev libxinerama-dev libxrandr-dev libxi-dev libasound2-dev mesa-common-dev libgl1-mesa-dev
#sudo apt-get install libdrm-dev libegl1-mesa-dev libgles2-mesa-dev libgbm-dev
pip3 install conan
export PATH="$HOME/.local/bin:$PATH"
````

### TFT and touch support
````bash
sudo pip3 install --upgrade adafruit-python-shell click
git clone https://github.com/adafruit/Raspberry-Pi-Installer-Scripts.git
cd Raspberry-Pi-Installer-Scripts
sudo python3 adafruit-pitft.py --display=28c --rotation=90 --install-type=fbcp --reboot=yes
````

### Build
````bash
killall learn_raylib

git fetch
git reset --hard HEAD
git pull

# Not strictly necessary, but I develop on a Mac
sed -i 's/..\/src\/include\/unicorn_body.obj/src\/include\/unicorn_body.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_horn.obj/src\/include\/unicorn_horn.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_mane.obj/src\/include\/unicorn_mane.obj/g' src/main.cpp
sed -i 's/..\/src\/include\/unicorn_tail.obj/src\/include\/unicorn_tail.obj/g' src/main.cpp

export CONAN_SYSREQUIRES_MODE=enabled
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target learn_raylib --clean-first -- -j 4

./learn_raylib
````
