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