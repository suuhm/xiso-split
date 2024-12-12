#!/bin/bash

echo "XISO-SPLIT - Getting xiso data from Redumped Xbox Games"
echo "               -----------------                       "
echo "              (c) 2021 - suuhmer                       "
echo
echo "Usage: $0 <DIR/DIR/*.iso> or $0 <DIR/DIR/xbox-redump.iso"
echo
echo "--------------------------------------------------------"
sleep 2 ; echo
echo "Taken idea and infos from issue:"
echo "https://github.com/XboxDev/extract-xiso/issues/32"
echo "--------------------------------------------------------"

_get_xiso()
{
FILEN=$(basename "$iso")
#OU="$DIR/"
echo "Progressing file: $FILEN"
echo 
echo "To -> ${OUDIR}/$FILEN"
sleep 6

#./extract-xiso -l ${DIR}/xiso.iso
#start dd dump: 
dd if="$iso" of=${OUDIR}/"$FILEN" skip=387k bs=1k status=progress

echo "Check up files"
sleep 2
echo "-------"

#TODO ftp upload / xbe file
}

#MAIN

_check_dir()
{
if [ -e $OUDIR ]; then
   echo "OK. XISO Dir exist already."
else
   mkdir -p ${OUDIR}
fi
}

if [ $1 == "--file" ]; then
    SHDIR=$(dirname "$2")
    OUDIR="${SHDIR}/XISO"
    _check_dir

    for iso in "$2"; do
        _get_xiso
    done
else
    SHDIR=$(dirname "$1")
    OUDIR="${SHDIR}/XISO"
    _check_dir

    for iso in $SHDIR/*.iso; do
        _get_xiso
    done
fi

echo "done"
exit 0
