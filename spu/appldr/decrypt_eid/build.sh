#!/bin/bash

export FLAGS="-g -O1 -Wall -fno-inline -ffreestanding -nostdlib -Wl,--build-id=none -static -eentry"

export CC=/mnt/datastore1/ps3/ps3dev/spu/bin/spu-gcc

$CC $FLAGS decrypt_eid.c -o decrypt_eid.elf || exit 1
$CC $FLAGS decrypt_eid.c -o decrypt_eid.bin -Wl,--oformat=binary || exit 1