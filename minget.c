#include "minget.h"

//replace args with struct
int minget(struct fsinfo fs) {
	FILE * srcFile;
	FILE * destFile;
	struct min_inode srcInode;

	//set destination to one provided, or stdout
	if (!fs.dstpath)
		destFile = stdout;

	else
		destFile = fopen(fs.dstpath, "w");
	
	//traverse filepath
	srcInode = traverseFiles(fs);

    /*check if filepath leads to a file,
	 if it leads to a directory abort*/
	if (isDir(srcInode)) {
		//malloc size of file; pray that your malloc isn't somehow being used
		char * fileContents = malloc(srcInode.size);

		//make calls to source less verbose
		srcFile = fs.diskimage;

		//move stream position to where it needs to be
		fseek(srcFile, srcInode.start, SEEK_SET);

		/*read to fileContents from source,
		 *write to destination from fileContents*/
		fread(fileContents, srcInode.size, 1, srcFile);
		fwrite(fileContents, srcInode.size, 1, destFile);

		//free stuff; close stuff
		fclose(destFile);
		fclose(srcFile);
		free(fileContents);

		return 0;
	}

	else {
		fprintf(stderr, "ERROR: destination is not a file. Exiting...\n");
		return -1;
	}
}

int main(int argc, char** argv) {
	struct fsinfo fs = parser(argc, argv, TRUE); 
	return minget(fs);
}