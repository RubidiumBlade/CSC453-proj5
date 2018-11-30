#ifndef MINIXFILES_H
#define MINIXFILES_H

#include <stdint.h>

#define DIRECT_ZONES 7

struct partitionTable {
    uint8_t bootind;
    uint8_t start_head;
    uint8_t start_sec;
    uint8_t start_cyl;
    uint8_t type;
    uint8_t end_head;
    uint8_t end_sec;
    uint8_t end_cyl;
    uint32_t lFirst;
    uint32_t size;
};

struct superblock { /* Minix Version 3 Superblock
                     * this structure found in fs/super.h
                     * in minix 3.1.1
                     */
    /* on disk. These fields and orientation are non–negotiable */
    uint32 t ninodes; /* number of inodes in this filesystem */
    uint16 t pad1; /* make things line up properly */
    int16 t i blocks; /* # of blocks used by inode bit map */
    int16 t z blocks; /* # of blocks used by zone bit map */
    uint16 t firstdata; /* number of first data zone */
    int16 t log zone size; /* log2 of blocks per zone */
    int16 t pad2; /* make things line up again */
    uint32 t max file; /* maximum file size */
    uint32 t zones; /* number of zones on disk */
    int16 t magic; /* magic number */
    int16 t pad3; /* make things line up again */
    uint16 t blocksize; /* block size in bytes */
    uint8 t subversion; /* filesystem sub–version */
};

struct inode {
    uint16 t mode; /* mode */
    uint16 t links; /* number or links */
    uint16 t uid;
    uint16 t gid;
    uint32 t size;
    int32 t atime;
    int32 t mtime;
    int32 t ctime;
    uint32 t zone[DIRECT ZONES];
    uint32 t indirect;
    uint32 t two indirect;
    uint32 t unused;
};


#endif