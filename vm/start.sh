#!/bin/bash

# Requires /boot/corepure64.gz from CorePure64-11.0.iso.

qemu-system-x86_64\
	-m 512\
	-kernel ../linux-5.4.3/arch/x86_64/boot/bzImage\
	-initrd corepure64.gz\
	-nic user,hostfwd=tcp:127.0.0.1:2222-:22
