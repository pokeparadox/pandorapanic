#!/bin/bash
mksquashfs ./built/font ./built/images ./built/music ./built/preview ./built/scripts ./built/sounds ./built/index.html ./bin/Pandora/PandoraPanic_R ./built/PXML.xml PandoraPanic.sfs
cat ./PandoraPanic.sfs ./built/PXML.xml ./built/images/ppicon.png > PandoraPanic.pnd
