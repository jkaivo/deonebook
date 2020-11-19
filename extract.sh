#!/bin/sh
D=/run/media/mmcblk0p1

$D/deonebook > $D/eonebook.key

exec $D/eonebook || shutdown -h now
