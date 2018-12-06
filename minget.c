#include "minget.h"

//replace args with struct
int minget(struct fsinfo fs) {
	FILE * destFile;
	struct min_inode srcInode;

	//set destination to one provided, or stdout
	if (!fs.dstpath)
		destFile = stdout;

	else
		destFile = fopen(fs.dstpath, "w");
	
	//traverse filepath
	srcInode = traverseFiles(&fs);

    /*check if filepath leads to a file,
	 if it leads to a directory abort*/
	if (isDir(srcInode)) {
		/*read to fileContents from source,
		 *write to destination from fileContents*/
		void * fileContents = collect_file(srcInode, fs, NULL);
		fwrite(fileContents, srcInode.size, 1, destFile);

		//free stuff; close stuff
		fclose(destFile);
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