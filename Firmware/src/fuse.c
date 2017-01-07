/**
 * \file fuse.c
 * \author
 */
#include <avr/io.h>

FUSES = {
    .low = (FUSE_CKSEL3 & FUSE_SUT0),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT
};
