#include "minixFiles.h"

/* when using partitioning, make sure partition table is valid 
 * return table if valid*/
struct partitionTable partitionValidity(File * diskImage, int part,
                                         unsigned int offset) {
    unsigned int firstPartition = 0x1BE + offset;
    unsigned int validityBits = firstPartition + sizeof(partitionTable);
    unsigned int currentPartition = firstParttition;
    //byte values that indicate if partition table is valid
    const unsigned int validityByte1 = 0x55;
    const unsigned int validityByte2 = 0xAA;
    void * partTable;
    void * testByte1;
    void * testByte2;

    //check to see if partition table it valid, if not exit
    fseek(diskimage, validityBits, SET_SEEK);
    fread(testByte1, sizeof(int), 1, diskimage);
    if ((int)testByte1 != validityByte1)
        exit(EXIT_FAILURE);

    fread(testByte2, sizeof(int), 1, diskimage);
    if((int)testByte2 != validityByte2)
        exit(EXIT_FAILURE);

    //loop through partitions
    for (int i=0; i < part; i++)
        currentPartition += sizeof(partitionTable);

    //load partition table into struct
    fseek(diskimage, currentPartition, SET_SEEK);
    fread(partTable, sizeof(partitionTable), 1, diskimage);
    return (partitionTable) partTable;
}

/* check to see if the found file is actually a file */
int fileValidity(char* file) {
	return 0;
}

/* traverse the filesystem until you find the requested file/directory */
struct min_inode traverseFiles(struct fsinfo fs) {
    //if part call partitionTable(part, 0)
    //if subpart call partitionTable(subpart, partition.start)

	return 0;
}

/* Takes in the arguments and parses them */
struct fsinfo parser(int argc, const char * argv[], int get) {
    struct fsinfo fs;
    char* opstring = "vp:s:";
    char arg;    

    fs.part = NULL;
    fs.subpart = NULL;
    fs.verbose = FALSE;

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
                exit(EXIT_FAILURE);
            }
        }
        if (arg == 's') {
            if (fs.part == NULL){
                fprintf(stderr, "Option -s requires option -p.");
                exit(EXIT_FAILURE);
            }
            fs.subpart = strtol(optarg, NULL, 10);
            if (fs.part == 0){
                fprintf(stderr, "Option -s requires an integer argument.");
                exit(EXIT_FAILURE);
            }
        }
    }
    fs.imagefile = argv[optind];
    fs.diskimage = fopen(fs.imagefile, 'r');
    if (get){
        fs.srcpath = argv[optind + 1];
        if (optind + 2 < argc) {
            fs.dstpath = argv[optind + 2];
        } else {
            fs.dstpath = NULL;
        }
        fs.filepath = NULL;
    } else {
        if (optind + 1 < argc){
            fs.filepath = argv[optind + 1];
        } else {
            fs.filepath = "/";
        }
        fs.srcpath = NULL;
        fs.dstpath = NULL;
    }

    ext_fsinfo(&fs);
    return fs;
}

int isdir(struct min_inode amiadir){
    if (FILE_TYPE_MASK & amiadir.mode == DIRECTORY_FILE_MASK){
        return TRUE;
    } else {
        return FALSE;
    }
}

int isregfile(struct min_inode amiregfile){
    if (FILE_TYPE_MASK & amiregfile.mode == REGULAR_FILE_MASK){
        return TRUE;
    } else {
        return FALSE;
    }
}

int read_directory(struct fsinfo fs, struct inode * inode_table, struct min_inode file, struct min_inode * found_files){
    struct dir_entry * collected_file = collect_file(file);
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
        printf('d');
    } else if (iisfile(file)){
        printf('-');
    } else {
        fprintf(stderr, "File is not a directory or a regular file.\n");
    }

    if (OWNR_R & file.mode){
        printf('r');
    } else {
        printf('-');
    }
    if (OWNR_W & file.mode){
        printf('w');
    } else {
        printf('-');
    }
    if (OWNR_X & file.mode){
        printf('x');
    } else {
        printf('-');
    }

    if (GRP_R & file.mode){
        printf('r');
    } else {
        printf('-');
    }
    if (GRP_W & file.mode){
        printf('w');
    } else {
        printf('-');
    }
    if (GRP_X & file.mode){
        printf('x');
    } else {
        printf('-');
    }

    if (USR_R & file.mode){
        printf('r');
    } else {
        printf('-');
    }
    if (USR_W & file.mode){
        printf('w');
    } else {
        printf('-');
    }
    if (USR_X & file.mode){
        printf('x');
    } else {
        printf('-');
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

void * collect_file(struct min_inode file, struct fsinfo fs, struct inode * inode_table){
    void * foundfile = malloc(file.size), * dummy = malloc(fs.zonesize), * pos = foundfile;
    int bytesleft, blocknum = 0;
    struct inode * inode_actual = &inode_table[file.inum];
    if (inode_table == NULL){
        inode_table = get_inode_table(fs);
    }
    for (bytesleft = file.size; bytesleft > fs.blocksize; bytesleft -= fs.blocksize){
        if (blocknum < 7){
            fseek(fs.diskimage, (inode_actual->zone[blocknum] * fs.zonesize) + fs->offset, SEEK_SET);
        } else if (blocknum <  / fs.zonesize)
        fread(dummy, fs.zonesize, 1, fs.diskimage);
        memcpy(foundfile, dummy, fs.zonesize);
    }
        
}
