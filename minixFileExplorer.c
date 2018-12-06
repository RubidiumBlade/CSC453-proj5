#include "minixFiles.h"

/* when using partitioning, make sure partition table is valid 
 * return table if valid*/
struct partitionTable partitionValidity(FILE * diskImage, int part,
                                         unsigned int offset) {
    unsigned int firstPartition = 0x1BE + offset;
    unsigned int validityBits = firstPartition + sizeof(partitionTable);
    unsigned int currentPartition = firstPartition;
    //byte values that indicate if partition table is valid
    const unsigned int validityByte1 = 0x55;
    const unsigned int validityByte2 = 0xAA;
    struct partitionTable * partTable = malloc(sizeof(struct partitionTable));
    void * testByte1 = malloc(sizeof(int));
    void * testByte2 = malloc(sizeof(int));
    int i;

    //check to see if partition table it valid, if not exit
    fseek(diskImage, validityBits, SEEK_SET);
    fread(testByte1, sizeof(int), 1, diskImage);
    if (*(int *)testByte1 != validityByte1) {
        fprintf(stderr, "Partition Table is invalid!\n");
        exit(EXIT_FAILURE);
    }

    fread(testByte2, sizeof(int), 1, diskImage);
    if(*(int *)testByte2 != validityByte2) {
        fprintf(stderr, "Partition Table is invalid!\n");
        exit(EXIT_FAILURE);
    }

    //loop through partitions
    for (i=0; i < part; i++)
        currentPartition += sizeof(struct partitionTable);

    //load partition table into struct
    fseek(diskImage, currentPartition, SEEK_SET);
    fread(partTable, sizeof(struct partitionTable), 1, diskImage);
    return *partTable;
}

/* traverse the filesystem until you find the requested file/directory */
struct min_inode traverseFiles(struct fsinfo * fs) {
    //if part call partitionTable(part, 0)
    //if subpart call partitionTable(subpart, partition.start)
    struct partitionTable partTable;
    unsigned int startByte = 0;
    if (fs->part > 0) {
        partTable = partitionValidity(fs->diskimage, fs->part, 0);
        //multiply by 512 because that's the size of a sector
        startByte = partTable.lFirst * 512;

        if(fs->verbose)
            printVerbose(&partTable, PARTITION);
    }

    if (fs->subpart > 0) {
        partTable = partitionValidity(fs->diskimage, fs->subpart, startByte);
        //multiply by 512 because that's the size of a sector
        startByte = partTable.lFirst * 512;

        if(fs->verbose)
            printVerbose(&partTable, SUBPARTITION);
    }

    //find superblock, start loc plus 1024
    startByte += 1024;

    //fill superblock struct, and relevant info in fsinfo struct
    struct superblock * sblock = malloc(sizeof(struct superblock));
    fseek(fs->diskimage, startByte, SEEK_SET);
    fread(sblock, sizeof(struct superblock), 1, fs->diskimage);
    fs->num_inodes = sblock->ninodes;
    fs->inode_table_start_block = sblock->i_blocks + sblock->z_blocks + 2;
    fs->blocksize = sblock->blocksize;
    fs->zonesize = sblock->blocksize << sblock->log_zone_size;

    //print verbose info if needed
    if(fs->verbose)
            printVerbose(sblock, SUPERBLOCK);

    //find inodetable and first inode
    struct inode* inodeTable = get_inode_table(*fs);
    struct min_inode * files = NULL;
    struct min_inode currentInode;
    int numFiles, i;
    char * pathName;

    strcpy(currentInode.filename, "/");
    currentInode.inum = 1;
    currentInode.mode = inodeTable[1].mode;
    currentInode.size = inodeTable[1].size;

    //follow the filepath
    while((pathName = strtok(fs->filepath, "/"))) {
        //follow inodeto driectory, follow pathname filename inode
        //keep going until you get NULL for pathname
        //check to make inodenum is >1 and <numinodes
        if (isdir(currentInode)) {
            fprintf(stderr, "Current inodeis not a directory!\n");
            exit(EXIT_FAILURE);
        }

        if (*pathName) {
            numFiles = read_directory(*fs, inodeTable, currentInode, files);

            //find inodewith corresponding pathname
            for (i=0; i < numFiles; i++) {
                if(!strncmp(pathName, files[i].filename, 60)) {
                    currentInode = files[i];
                    break;
                }
            }

            if (i == numFiles) {
                fprintf(stderr, "File or directory not found!\n");
                exit(EXIT_FAILURE);
            }

            if (currentInode.inum < 1 || currentInode.inum >= fs->num_inodes) {
                fprintf(stderr, "Current inodeis illegal!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (fs->verbose) {
        int inum = currentInode.inum;
        struct inode verboseInode = inodeTable[inum];
        printVerbose(&verboseInode, INODE);
    }

    return currentInode;
}

/* Takes in the arguments and parses them */
struct fsinfo parser(int argc, char * const * argv, int get) {
    struct fsinfo fs;
    char* opstring = "vp:s:";
    char arg;    

    fs.part = -1;
    fs.subpart = -1;
    fs.verbose = FALSE;

    if ((argc < 2) || (argc < 3 && get)){
       printHelpText(get);
       exit(EXIT_FAILURE);
    }

    while ((arg = getopt(argc, argv, opstring)) != -1){
        if (arg == '?') {
            exit(EXIT_FAILURE);
        }
        if (arg == 'v') {
            fs.verbose = TRUE;
        }
        if (arg == 'p') {
            fs.part = strtol(optarg, NULL, 10);
            if (fs.part == 0){
                fprintf(stderr, "Option -p requires an integer argument.");
                printHelpText(get);
                exit(EXIT_FAILURE);
            }
        }
        if (arg == 's') {
            if (fs.part == -1){
                fprintf(stderr, "Option -s requires option -p.");
                printHelpText(get);
                exit(EXIT_FAILURE);
            }
            fs.subpart = strtol(optarg, NULL, 10);
            if (fs.part == 0){
                fprintf(stderr, "Option -s requires an integer argument.");
                printHelpText(get);
                exit(EXIT_FAILURE);
            }
        }
    }
    fs.imagefile = malloc(strlen(argv[optind]) + 1);
    strcpy(fs.imagefile, argv[optind]);
    fs.diskimage = fopen(fs.imagefile, "r");
    if (fs.diskimage == NULL){
       perror("fopen");
       exit(EXIT_FAILURE);
    }
    if (get){
        strcpy(fs.srcpath, argv[optind + 1]);
        if (optind + 2 < argc) {
            strcpy(fs.dstpath, argv[optind + 2]);
        } else {
            fs.dstpath = NULL;
        }
        fs.filepath = NULL;
    } else {
        if (optind + 1 < argc){
            strcpy(fs.filepath, argv[optind + 1]);
        } else {
            fs.filepath = "/";
        }
        fs.srcpath = NULL;
        fs.dstpath = NULL;
    }

    ext_fsinfo(&fs);
    return fs;
}

void printHelpText(int get){
    if (get){
        printf("usage: minget [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n"
            "Options:\n"
            "-p part --- select partition for filesystem (default: none)\n"
            "-s sub --- select subpartition for filesystem (default: none)\n"
            "-h help --- print usage information and exit\n"
            "-v verbose --- increase verbosity level\n");
    } else {
        printf("usage: minls [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n"
            "Options:\n"
            "-p part --- select partition for filesystem (default: none)\n"
            "-s sub --- select subpartition for filesystem (default: none)\n"
            "-h help --- print usage information and exit\n"
            "-v verbose --- increase verbosity level\n");
    }
}

int isdir(struct min_inode amiadir){
    if ((FILE_TYPE_MASK & amiadir.mode) == DIRECTORY_FILE_MASK){
        return TRUE;
    } else {
        return FALSE;
    }
}

int isregfile(struct min_inode amiregfile){
    if ((FILE_TYPE_MASK & amiregfile.mode) == REGULAR_FILE_MASK){
        return TRUE;
    } else {
        return FALSE;
    }
}

int read_directory(struct fsinfo fs, struct inode* inode_table, struct min_inode file, struct min_inode * found_files){
    struct dir_entry * collected_file = collect_file(file, fs, inode_table);
    int i, inode_num;
    found_files = malloc(file.size);
    for (i = 0; i < file.size / sizeof(struct dir_entry); i++){
        strncpy(found_files[i].filename, collected_file[i].name, 60);
        inode_num = collected_file[i].inode;
        found_files[i].mode = inode_table[inode_num].mode;
        found_files[i].size = inode_table[inode_num].size;
        found_files[i].inum = inode_num;
    }
    return file.size / sizeof(struct dir_entry);
}

void printfile(struct min_inode file, int print_filename){
    if (isdir(file)){
        printf("d");
    } else if (isregfile(file)){
        printf("-");
    } else {
        fprintf(stderr, "File is not a directory or a regular file.\n");
    }

    if (OWNR_R & file.mode){
        printf("r");
    } else {
        printf("-");
    }
    if (OWNR_W & file.mode){
        printf("w");
    } else {
        printf("-");
    }
    if (OWNR_X & file.mode){
        printf("x");
    } else {
        printf("-");
    }

    if (GRP_R & file.mode){
        printf("r");
    } else {
        printf("-");
    }
    if (GRP_W & file.mode){
        printf("w");
    } else {
        printf("-");
    }
    if (GRP_X & file.mode){
        printf("x");
    } else {
        printf("-");
    }

    if (USR_R & file.mode){
        printf("r");
    } else {
        printf("-");
    }
    if (USR_W & file.mode){
        printf("w");
    } else {
        printf("-");
    }
    if (USR_X & file.mode){
        printf("x");
    } else {
        printf("-");
    }

    printf(" %9d", file.size);
    if (print_filename){
        printf(" %.60s", file.filename);
        printf("\n");
    }
}

void ext_fsinfo(struct fsinfo * fs){
    struct superblock * sblk = malloc(sizeof(struct superblock));
    fseek(fs->diskimage, 1024, SEEK_SET);
    fread((void *) sblk, sizeof(struct superblock), 1, fs->diskimage);
    fs->num_inodes = sblk->ninodes;
    fs->inode_table_start_block = sblk->i_blocks + sblk->z_blocks + 2;
    fs->blocksize = sblk->blocksize;
    fs->zonesize = sblk->blocksize << sblk->log_zone_size;
}

void * collect_file(struct min_inode file, struct fsinfo fs, struct inode* inode_table){
    void * foundfile = malloc(file.size), * dummy = malloc(fs.zonesize), * pos = foundfile;
    int bytesleft, zonenum = 0, num_ino_idi = fs.zonesize / sizeof(uint32_t), hole;
    struct inode* inode_actual = &inode_table[file.inum];
    uint32_t * indirect_table = malloc(fs.zonesize);
    if (inode_table == NULL){
        inode_table = get_inode_table(fs);
    }
    
    for (bytesleft = file.size; bytesleft > 0; bytesleft -= fs.zonesize){
        hole = FALSE;
        if (zonenum < 7){ /* if in direct zones */
            /* just seek to the correct zone */
            if (inode_actual->zone[zonenum]){
                fseek(fs.diskimage, (inode_actual->zone[zonenum] * fs.zonesize) + fs.offset, SEEK_SET);
            } else {
                hole = TRUE;
            }
        } else if (zonenum < (num_ino_idi + 7)){ /* in indirect zones */
            /* get the indierct zone */
            fseek(fs.diskimage, (inode_actual->indirect * fs.zonesize) + fs.offset, SEEK_SET);
            fread(indirect_table, fs.zonesize, 1, fs.diskimage);
            if (indirect_table[zonenum - 7]) {
                fseek(fs.diskimage, (indirect_table[zonenum - 7] * fs.zonesize) + fs.offset, SEEK_SET);
            } else {
                hole = TRUE;
            }
        } else if ((zonenum < pow(num_ino_idi, 2) + 7)){ /* in double indierct zones */
            fseek(fs.diskimage, (inode_actual->two_indirect * fs.zonesize) + fs.offset, SEEK_SET);
            fread(indirect_table, fs.zonesize, 1, fs.diskimage);
            fseek(fs.diskimage, (indirect_table[(zonenum - 7 - num_ino_idi) / num_ino_idi] * fs.zonesize) + fs.offset, SEEK_SET);
            fread(indirect_table, fs.zonesize, 1, fs.diskimage);
            if (indirect_table[(zonenum - 7 - num_ino_idi) % num_ino_idi]) {
                fseek(fs.diskimage, (indirect_table[(zonenum - 7 - num_ino_idi) % num_ino_idi] * fs.zonesize) + fs.offset, SEEK_SET);
            } else {
                hole = TRUE;
            }
        } else {
            fprintf(stderr, "Oversized file. Aborting...");
            exit(EXIT_FAILURE);
        }
        if (hole == FALSE){
            if (bytesleft > fs.zonesize){
                fread(dummy, fs.zonesize, 1, fs.diskimage);
                memcpy(pos, dummy, fs.zonesize); 
            } else {
                fread(dummy, bytesleft, 1, fs.diskimage);
                memcpy(pos, dummy, bytesleft);
            }
        } else {
            if (bytesleft > fs.zonesize){
                memset(pos, 0, fs.zonesize); 
            } else {
                memset(pos, 0, bytesleft);
            }
        }
        zonenum += 1;
        pos += fs.zonesize;
    }

    return foundfile;    
}

struct inode* get_inode_table(struct fsinfo fs){
    struct inode* inode_table = malloc(sizeof(struct inode) * fs.num_inodes);
    fseek(fs.diskimage, (fs.inode_table_start_block * fs.blocksize) + fs.offset, SEEK_SET);
    fread(inode_table, fs.num_inodes * 64, 1, fs.diskimage);
    return inode_table;
}
