#!/bin/bash

# qcfwgen0.sh <work_dir>
# work_dir must be in same directory as this file

# contents of <work_dir> must be:

# inros.bin (OFW)

# lv0.elf (OFW)

# lv1.elf.orig (OFW)
# lv1.elf (OFW or patched)

# lv2_kernel.elf.orig (OFW)
# lv2_kernel.elf (OFW or patched)

# lv2hashgen.elf (no longer needed)

if [[ $# -eq 0 ]] ; then
    echo 'missing args'
    exit 1
fi

export ROOT_DIR=$PWD
echo ROOT_DIR=$ROOT_DIR

export WORK_DIR=$1
echo WORK_DIR=$WORK_DIR

echo Building stage...
cd $ROOT_DIR/BadWDSD-Stage || exit 1
./build.sh || exit 1

echo Building tools...

cd $ROOT_DIR/tools/coreos_tools || exit 1
./build.sh || exit 1

cd $ROOT_DIR/tools/lv0gen || exit 1
./build.sh || exit 1

cd $ROOT_DIR/tools/lv1gen || exit 1
./build.sh || exit 1

cd $ROOT_DIR/tools/lv2gen || exit 1
./build.sh || exit 1

cd $ROOT_DIR/tools/zgen || exit 1
./build.sh || exit 1

cd $ROOT_DIR/tools/dtbImage_ps3_bin_to_elf || exit 1
./build.sh || exit 1

cd $ROOT_DIR || exit 1
cd $WORK_DIR || exit 1

echo Delete workdir temp...
rm -rf temp

rm lv0.stage2j.elf
rm lv0.stage2j.zelf

rm lv1.stage3j3ja3jz5j.elf

rm lv2_kernel.diff

rm outros.bin
rm CoreOS.bin

echo Delete workdir inros...
rm -rf inros

echo Delete workdir outros...
rm -rf outros

echo Copying needed files to temp...
mkdir temp || exit 1

cp $ROOT_DIR/BadWDSD-Stage/Stage2j.bin temp/Stage2j.bin || exit 1
cp $ROOT_DIR/BadWDSD-Stage/Stage3j.bin temp/Stage3j.bin || exit 1
cp $ROOT_DIR/BadWDSD-Stage/Stage3ja.bin temp/Stage3ja.bin || exit 1
cp $ROOT_DIR/BadWDSD-Stage/Stage3jz.bin temp/Stage3jz.bin || exit 1
cp $ROOT_DIR/BadWDSD-Stage/Stage5j.bin temp/Stage5j.bin || exit 1

cp $ROOT_DIR/tools/coreos_tools/coreos_tools temp/coreos_tools || exit 1
cp $ROOT_DIR/tools/lv0gen/lv0gen temp/lv0gen || exit 1
cp $ROOT_DIR/tools/lv1gen/lv1gen temp/lv1gen || exit 1
cp $ROOT_DIR/tools/lv2gen/lv2gen temp/lv2gen || exit 1
cp $ROOT_DIR/tools/zgen/zgen temp/zgen || exit 1
cp $ROOT_DIR/tools/dtbImage_ps3_bin_to_elf/dtbImage_ps3_bin_to_elf temp/dtbImage_ps3_bin_to_elf || exit 1

echo Extracting inros.bin...
mkdir inros

temp/coreos_tools extract_coreos inros.bin inros || exit 1

echo Install stage2j to lv0.elf...
temp/lv0gen lv0gen lv0.elf lv0.stage2j.elf temp/Stage2j.bin || exit 1

echo Generate lv0.stage2j.zelf...
temp/zgen zelf_gen lv0.stage2j.elf lv0.stage2j.zelf || exit 1

echo Install stage3j/3ja/3jz/5j to lv1.elf...
temp/lv1gen lv1gen lv1.elf lv1.stage3j3ja3jz5j.elf temp/Stage3j.bin temp/Stage3ja.bin temp/Stage3jz.bin temp/Stage5j.bin || exit 1

echo Generate lv1.diff
temp/lv1gen lv1diff lv1.elf.orig lv1.stage3j3ja3jz5j.elf lv1.diff || exit 1

echo Generate lv2_kernel.diff
temp/lv2gen lv2diff lv2_kernel.elf.orig lv2_kernel.elf lv2_kernel.diff || exit 1

echo Copying inros to outros...
cp -a inros outros || exit 1

echo Deleting creserved_0...
rm outros/creserved_0

#echo Deleting hdd_copy.self...
#rm outros/hdd_copy.self

echo Copying lv0.stage2j.zelf to outros/lv0.zelf...
cp -a lv0.stage2j.zelf outros/lv0.zelf || exit 1

echo Copying lv1.diff to outros/lv1.diff...
cp -a lv1.diff outros/lv1.diff || exit 1

echo Copying lv2_kernel.diff to outros/lv2_kernel.diff...
cp -a lv2_kernel.diff outros/lv2_kernel.diff || exit 1

#echo Copying lv2hashgen.elf to outros/lv2hashgen.elf...
#cp -a lv2hashgen.elf outros/lv2hashgen.elf || exit 1

read -p "Modify outros now then press ENTER to continue"

echo Generate CoreOS.bin...
temp/coreos_tools create_coreos outros CoreOS.bin || exit 1