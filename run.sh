#!/bin/bash
echo "Next command: 'make clean'"
make clean
echo "'make clean' command executed"
echo "Next command: 'make all'"
make all
echo "'make all' command executed"
echo "QEMU will be started!"
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
