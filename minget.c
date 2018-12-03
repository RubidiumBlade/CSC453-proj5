#include "minget.h"

//replace args with struct
int minget(char* imagefile, char* srcpath, char* dstpath, int part, int subpart, int verbose) {
	FILE * srcFile;
	FILE * destFile;
	min_inode * srcIode;
	min_inode * destInode;

	//copy contents of file to destination, or stdout
	if (!dstpath)
		destination = stdout;

	/*if partitioing, check partition validity
	 follow directory path, check if filepath leads to a file,
	 if it leads to a directory abort*/

	return 0;
}

int main(int args, char** argv) {
	//insert parser here
	//call minget
	return 0;
}