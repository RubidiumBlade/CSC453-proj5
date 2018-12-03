#include "minixFiles.h"

/* when using partitioning, make sure partition table is valid */
int partitionValidity(int part) {
	return 0;
}

/* check to see if the found file is actually a file */
int fileValidity(char* file) {
	return 0;
}

/* traverse the filesystem until you find the requested file/directory */
struct min_inode* traverseFiles(int part, int subpart, char* imgfile, char* srcpath) {
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
