#!/bin/sh
D=/run/media/mmcblk0p1

$D/deonebook -R > $D/eonebook.hex

exec $D/eonebook || shutdown -h now
