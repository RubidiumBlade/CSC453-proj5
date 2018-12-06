#ifndef MINIXFILES_H
#define MINIXFILES_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

/* file type masks */

#define FILE_TYPE_MASK 0170000
#define REGULAR_FILE_MASK 0100000
#define DIRECTORY_FILE_MASK 0040000
#define OWNR_R 0000400
#define OWNR_W 0000200
#define OWNR_X 0000100
#define GRP_R 0000040
#define GRP_W 0000020
#define GRP_X 0000010
#define USR_R 0000004
#define USR_W 0000002
#define USR_X 0000001

//mode values for verbose
#define SUPERBLOCK 1
#define PARTITION 2
#define SUBPARTITION 3
#define INODE 4

#define DIRECT_ZONES 7

struct __attribute__ ((__packed__)) partitionTable {
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

struct __attribute__ ((__packed__)) superblock { 
                    /* Minix Version 3 Superblock
                     * this structure found in fs/super.h
                     * in minix 3.1.1
                     */
    /* on disk. These fields and orientation are non–negotiable */
    uint32_t ninodes; /* number of inodes in this filesystem */
    uint16_t pad1; /* make things line up properly */
    int16_t i_blocks; /* # of blocks used by inode bit map */
    int16_t z_blocks; /* # of blocks used by zone bit map */
    uint16_t firstdata; /* number of first data zone */
    int16_t log_zone_size; /* log2 of blocks per zone */
    int16_t pad2; /* make things line up again */
    uint32_t max_file; /* maximum file size */
    uint32_t zones; /* number of zones on disk */
    int16_t magic; /* magic number */
    int16_t pad3; /* make things line up again */
    uint16_t blocksize; /* block size in bytes */
    uint8_t subversion; /* filesystem sub–version */
};

struct inode {
    uint16_t mode; /* mode */
    uint16_t links; /* number or links */
    uint16_t uid;
    uint16_t gid;
    uint32_t size;
    int32_t atime;
    int32_t mtime;
    int32_t ctime;
    uint32_t zone[DIRECT_ZONES];
    uint32_t indirect;
    uint32_t two_indirect;
    uint32_t unused;
};

struct fsinfo {
    char * imagefile;
    char * filepath;
    char * srcpath;
    char * dstpath;
    int part;
    int subpart;
    int verbose;
    FILE * diskimage;
    int num_inodes;
    int inode_table_start_block;
    int blocksize;
    int zonesize;
    int offset;
};

struct min_inode {
    char filename[60];
    int size;
    int inum;
    uint16_t mode;
};

struct __attribute__ ((__packed__)) dir_entry {
    uint32_t inode;
    char name[60];
};

int printVerbose(void * printInfo, int mode);
struct fsinfo parser(int argc, char * const * argv, int get);
struct partitionTable partitionTable(int part, unsigned int offset);
int fileValidity(char * file);
struct min_inode traverseFiles(struct fsinfo * fs);
int isdir(struct min_inode amiadir); /* done */
int isregfile(struct min_inode amiregfile); /* done */
int read_directory(struct fsinfo fs, struct inode * inode_table, struct min_inode file, struct min_inode * found_files); /* done */
void printfile(struct min_inode file, int print_filename); /* printfile */
void * collect_file(struct min_inode file, struct fsinfo fs, struct inode * inode_table); /* done */
struct inode * get_inode_table(struct fsinfo fs); /* done */
void ext_fsinfo(struct fsinfo * fs); /* depricated */
void printHelpText(int get);

#endif
