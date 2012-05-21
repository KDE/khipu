#!bin/sh
$EXTRACTRC `find -name '*.ui' -o -name '*.kcfg'` >> rc.cpp
$XGETTEXT `find -name '*.h' -o -name '*.cpp'` -o $podir/gplacs.pot
