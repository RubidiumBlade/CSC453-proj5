#include "minget.h"

//replace args with struct
int minget(struct fsinfo fs) {
	FILE * srcFile;
	FILE * destFile;
	struct min_inode * srcIode;
	struct min_inode * destInode;

	//copy contents of file to destination, or stdout
	if (!fs.dstpath) {
		destFile = stdout;
	}
	
	/*if partitioing, check partition validity
	 follow directory path, check if filepath leads to a file,
	 if it leads to a directory abort*/

	return 0;
}

int main(int argc, char** argv) {
	struct fsinfo fs = parser(argc, argv, TRUE);
	//insert parser here
	//call minget
	return 0;
}