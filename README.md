[![Build](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml/badge.svg)](https://github.com/twelvechairssoftware/learn_raylib/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=twelvechairssoftware_learn_raylib&metric=alert_status)](https://sonarcloud.io/dashboard?id=twelvechairssoftware_learn_raylib)

TFT support
````bash
sudo curl https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/pitft-fbcp.sh >pitft-fbcp.sh
sudo bash pitft-fbcp.sh
````

Raspberry Pi (buster)
````bash
sudo apt-get install -y build-essential git python3-pip curl libgl-dev libglu1-mesa-dev libegl-dev
pip3 install conan
export PATH="$HOME/.local/bin:$PATH"

sudo apt install snapd
# REBOOT REQUIRED
sudo snap install core
sudo snap install cmake --classic
````

Build
````shell
export CONAN_SYSREQUIRES_MODE=enabled
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G "CodeBlocks - Unix Makefiles" .
cmake --build . --target learn_raylib --clean-first -- -j 4
````