#include "minixFiles.h"

int printVerbose(void * printInfo, int mode) {
	switch (mode) {
		case SUPERBLOCK :
			fprintf(stderr, "Superblock contents:\nStoredFields:\n");
			fprintf(stderr, "   ninodes: %d\n", printInfo->ninodes);
			fprintf(stderr, "   pad1: %d\n", printInfo->pad1);
			fprintf(stderr, "   i_blocks: %d\n", printInfo->i_blocks);
			fprintf(stderr, "   z_blocks: %d\n", printInfo->z_blocks);
			fprintf(stderr, "   firstdata: %d\n", printInfo->firstdata);
			fprintf(stderr, "   log_zone_size: %d\n", printInfo->log_zone_size);
			fprintf(stderr, "   pad2: %d\n", printInfo->pad2);
			fprintf(stderr, "   max_file: %d\n", printInfo->max_file);
			fprintf(stderr, "   zones: %d\n", printInfo->zones);
			fprintf(stderr, "   magic: %d\n", printInfo->magic);
			fprintf(stderr, "   pad3: %d\n", printInfo->pad3);
			fprintf(stderr, "   blocksize: %d\n", printInfo->blocksize);
			fprintf(stderr, "   subversion: %d\n\n", printInfo->subversion);

		case PARTITION :
			fprintf(stderr, "Partition contents:\nStoredFields:\n");
			fprintf(stderr, "   start_head: %d\n", printInfo->start_head);
			fprintf(stderr, "   start_sec: %d\n", printInfo->start_sec);
			fprintf(stderr, "   start_cyl: %d\n", printInfo->ninodes);
			fprintf(stderr, "   type: %d\n", printInfo->type);
			fprintf(stderr, "   end_head: %d\n", printInfo->end_head);
			fprintf(stderr, "   end_sec: %d\n", printInfo->end_sec);
			fprintf(stderr, "   end_cyl: %d\n", printInfo->end_cyl);
			fprintf(stderr, "   lFirst: %d\n", printInfo->lFirst);
			fprintf(stderr, "   size: %d\n\n", printInfo->size);


		case SUBPARTITION :
			fprintf(stderr, "Subpartition contents:\nStoredFields:\n");
			fprintf(stderr, "   start_head: %d\n", printInfo->start_head);
			fprintf(stderr, "   start_sec: %d\n", printInfo->start_sec);
			fprintf(stderr, "   start_cyl: %d\n", printInfo->ninodes);
			fprintf(stderr, "   type: %d\n", printInfo->type);
			fprintf(stderr, "   end_head: %d\n", printInfo->end_head);
			fprintf(stderr, "   end_sec: %d\n", printInfo->end_sec);
			fprintf(stderr, "   end_cyl: %d\n", printInfo->end_cyl);
			fprintf(stderr, "   lFirst: %d\n", printInfo->lFirst);
			fprintf(stderr, "   size: %d\n\n", printInfo->size);

		case INODE :
			fprintf(stderr, "Subpartition contents:\nStoredFields:\n");
			fprintf(stderr, "   links: %d\n", printInfo->links);
			fprintf(stderr, "   uid: %d\n", printInfo->uid);
			fprintf(stderr, "   gid: %d\n", printInfo->gid);
			fprintf(stderr, "   size: %d\n", printInfo->size);
			fprintf(stderr, "   atime: %s\n", ctime(printInfo->atime));
			fprintf(stderr, "   mtime: %s\n", ctime(printInfo->mtime));
			fprintf(stderr, "   ctime: %s\n", ctime(printInfo->ctime));
			for (int i=0; i<DIRECT_ZONES; i++)
				fprintf(stderr, "   zone[%d]: %d\n", i, printInfo->zone[i]);

			fprintf(stderr, "   indirect: %d\n", printInfo->start_head);
			fprintf(stderr, "   two_indirect: %d\n", printInfo->start_head);
			fprintf(stderr, "   unused: %d\n", printInfo->start_head);


		default :
			fprintf(stderr, "Error: invalid mode given to print verbose...\n");
			exit(EXIT_FAILURE);
	}

	return 0;
}