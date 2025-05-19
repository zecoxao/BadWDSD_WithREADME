# BadWDSD_WithREADME
BadWDSD With README, Original Credits to Kafuu

## Pinouts 

# Main Pinout (Red Ones are Important, Black Ones Are Secondary)

![Main Pinout, Only 4 Pins are important, rest are dipswitches or debug pins for info](https://i.imgur.com/xw6f5FA.jpeg)

## Secondary Pinouts

![CMD_CLK](https://i.imgur.com/2gyw7on.jpeg)
![SC_TX_SC_RX](https://i.imgur.com/wmMCW19.jpeg)

# Requirements

PSL1GHT/PS3TOOLCHAIN (see: https://www.mediafire.com/file/i48teby46wtpbg4/ps3dev_27122022.tar.gz/file )

# Instructions

sudo tar -xf ps3dev_27122022.tar.gz -C /usr/local/

add to ~/.bashrc:

export PS3DEV=/usr/local/ps3dev

export PSL1GHT=$PS3DEV

export PATH=$PATH:$PS3DEV/bin

export PATH=$PATH:$PS3DEV/ppu/bin

export PATH=$PATH:$PS3DEV/spu/bin

sudo apt install gcc-arm-none-eabi gcc-powerpc64-linux-gnu

git clone --recursive https://github.com/zecoxao/BadWDSD_WithREADME

cd BadWDSD_WithREADME

# qcfwgen0.sh <work_dir>
# work_dir must be in same directory as this file

# contents of <work_dir> must be:

## inros.bin (OFW)

## lv0.elf (OFW)

## lv1.elf.orig (OFW)
# lv1.elf (OFW or patched)

## lv2_kernel.elf.orig (OFW)
# lv2_kernel.elf (OFW or patched)

## lv2hashgen.elf (no longer needed)
