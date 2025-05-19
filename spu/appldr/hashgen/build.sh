#!/bin/bash

export FLAGS="-g -O1 -Wall -fno-inline -ffreestanding -nostdlib -Wl,--build-id=none -static -eentry"

export CC=/mnt/datastore1/ps3/ps3dev/spu/bin/spu-gcc

$CC $FLAGS hashgen.c -o hashgen.elf || exit 1
$CC $FLAGS hashgen.c -o hashgen.bin -Wl,--oformat=binary || exit 1
