#include "minixFiles.h"

/* when using partitioning, make sure partition table is valid 
 * return table if valid*/
struct partitionTable partitionValidity(int part, unsigned int offset) {
    unsigned int firstPartition = 0x1BE + offset;
    unsigned int validityBits = firstPartition + 64;
    unsigned int currentPartition = firstParttition;

    //check to see if partition table it valid, if not exit

    //loop through partitions
    for (int i=0; i < part; i++)
        currentPartition += sizeof(partitionTable);

    //load partition table into struct

	return 0;
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
