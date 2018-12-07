#include "minixFiles.h"

int main(int argc, char* const *argv){
    struct min_inode * files = NULL;
    int numfiles, i;
    
    struct fsinfo fs = parser(argc, argv, FALSE);

    struct min_inode file_me = traverseFiles(&fs);
    files = malloc(file_me.size);

    if (isdir(file_me)){
        numfiles = read_directory(fs, fs.inodeTable, file_me, files);
        printf(fs.filepath);
        printf(":\n");

        printf("%d\n", numfiles);
        for (i = 0; i < numfiles; i++){
            printf("%d\n", files[i].inum);
           if (files[i].inum){
            printfile(files[i], TRUE);
           }
        }
        exit(EXIT_SUCCESS);
    } else if (isregfile(file_me)){
        printfile(file_me, FALSE);
        printf(" ");
        printf(fs.filepath);
        printf("\n");
        exit(EXIT_SUCCESS);
    } else {
        fprintf(stderr, 
              "The specified file is not a regular file or a directory.\n");
        exit(EXIT_FAILURE);
    }

}

