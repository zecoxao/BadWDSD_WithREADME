# BadWDSD_WithREADME
BadWDSD With README, Original Credits to Kafuu

# Debug UART Speed

576000 

## Pinouts 

# Main Pinout (Red Ones are Important, Black Ones Are Secondary)

![Main Pinout, Only 4 Pins are important, rest are dipswitches or debug pins for info](https://i.imgur.com/xw6f5FA.jpeg)
![Main Pinout](https://i.imgur.com/OgeFxGG.jpeg)

## Secondary Pinouts

![CMD_CLK](https://i.imgur.com/2gyw7on.jpeg)
![SC_TX_SC_RX](https://i.imgur.com/wmMCW19.jpeg)

# DipSwitch Pinout Meaning
	
SC_BYPASS = Pico in passive mode, it won't send sc command by itself (auth/banksel/recovery). And will only listen, waiting for trigger. (sc forwarding still works) This is for debug purpose only.

RECOVERY is for going into recovery. (Literally. Also used for downgrade.)

HOLD is to disable modchip

BANKSEL is to select ros bank. 

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

# precompiled

https://www.mediafire.com/file/w4txau3a2owtq8p/Stagex_20250529.bin/file

https://www.mediafire.com/file/b01sqkr4tv0g4v3/BadWDSD_SW_x32_20250529.uf2/file

https://www.mediafire.com/file/dnbd9w17730072f/BadWDSD_SW_x32_Zero_20250529.uf2/file

https://www.mediafire.com/file/w7wpjyn4gp5vvbn/qcfw-492-cex_dev_blind_20250530.zip/file

https://www.mediafire.com/file/ojyiyelur0wqgio/qcfw-492-cex_CoreOS_20250530.bin/file
