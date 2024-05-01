## install C library
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-mixer-dev

## Run the piano in linux
gcc cpiano.c -o example -lSDL2 -lSDL2_mixer
./example
