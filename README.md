# BadWDSD_WithREADME
BadWDSD With README, Original Credits to Kafuu

## Pinouts 

# Main Pinout

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

sudo apt install gcc-arm-none-eabi

cd BadWDSD

bash build.sh

cd ..