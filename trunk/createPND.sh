#!/bin/bash
#CLEAR old files
rm ./PandoraPanic.sfs
rm ./pandorapanic.pnd
#build new pnd
mksquashfs ./built/font ./built/images ./built/music ./built/preview ./built/scripts ./built/sounds ./built/index.html ./bin/Pandora/PandoraPanic_R ./resources_pandora/PXML.xml ./resources_pandora/PandoraPanic.sh PandoraPanic.sfs
cat ./PandoraPanic.sfs ./resources_pandora/PXML.xml ./built/images/ppicon.png > pandorapanic.pnd
