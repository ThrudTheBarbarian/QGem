#!/bin/sh
# 
# rscdump -f $HOME/src/QT/QGem/gemd/resources/icons1.rsc -l -m cam -e
# cat G_CICON_CAMERA.pam | pamtopng > /tmp/tst.png
# open /tmp/tst.png
# rm G_CICON_CAMERA.pam

 
# rscdump -f $HOME/Downloads/icons_13/PERI_APP.RSC -l -m cd3 -e
# cat G_CICON_CD3_1.pam | pamtopng > /tmp/tst.png
# open /tmp/tst.png
# rm G_CICON_CD3_?.pam

rm -f *.pam
rscdump -f $HOME/Downloads/fbb_154/cicons.rsc -l -m $1 -e
cat *_1.pam | pamtopng > /tmp/tst.png
open /tmp/tst.png
rm -f *.pam
