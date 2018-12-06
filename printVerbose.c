#include "minixFiles.h"

int printVerbose(void * printInfo_v, int mode) {
	struct superblock * printInfo_su;
	struct partitionTable * printInfo_p;
	struct inode* printInfo_i;
	switch (mode) {
		case SUPERBLOCK :
			printInfo_su = (struct superblock *) printInfo_v;
			fprintf(stderr, "Superblock contents:\nStoredFields:\n");
			fprintf(stderr, "   ninodes: %d\n", printInfo_su->ninodes);
			fprintf(stderr, "   pad1: %d\n", printInfo_su->pad1);
			fprintf(stderr, "   i_blocks: %d\n", printInfo_su->i_blocks);
			fprintf(stderr, "   z_blocks: %d\n", printInfo_su->z_blocks);
			fprintf(stderr, "   firstdata: %d\n", printInfo_su->firstdata);
			fprintf(stderr, "   log_zone_size: %d\n", printInfo_su->log_zone_size);
			fprintf(stderr, "   pad2: %d\n", printInfo_su->pad2);
			fprintf(stderr, "   max_file: %d\n", printInfo_su->max_file);
			fprintf(stderr, "   zones: %d\n", printInfo_su->zones);
			fprintf(stderr, "   magic: %d\n", printInfo_su->magic);
			fprintf(stderr, "   pad3: %d\n", printInfo_su->pad3);
			fprintf(stderr, "   blocksize: %d\n", printInfo_su->blocksize);
			fprintf(stderr, "   subversion: %d\n\n", printInfo_su->subversion);

		case PARTITION :
			printInfo_p = (struct partitionTable *) printInfo_v;
			fprintf(stderr, "Partition contents:\nStoredFields:\n");
			fprintf(stderr, "   start_head: %d\n", printInfo_p->start_head);
			fprintf(stderr, "   start_sec: %d\n", printInfo_p->start_sec);
			fprintf(stderr, "   start_cyl: %d\n", printInfo_p->start_cyl);
			fprintf(stderr, "   type: %d\n", printInfo_p->type);
			fprintf(stderr, "   end_head: %d\n", printInfo_p->end_head);
			fprintf(stderr, "   end_sec: %d\n", printInfo_p->end_sec);
			fprintf(stderr, "   end_cyl: %d\n", printInfo_p->end_cyl);
			fprintf(stderr, "   lFirst: %d\n", printInfo_p->lFirst);
			fprintf(stderr, "   size: %d\n\n", printInfo_p->size);


		case SUBPARTITION :
			printInfo_p = (struct partitionTable *) printInfo_v;
			fprintf(stderr, "Subpartition contents:\nStoredFields:\n");
			fprintf(stderr, "   start_head: %d\n", printInfo_p->start_head);
			fprintf(stderr, "   start_sec: %d\n", printInfo_p->start_sec);
			fprintf(stderr, "   start_cyl: %d\n", printInfo_p->start_cyl);
			fprintf(stderr, "   type: %d\n", printInfo_p->type);
			fprintf(stderr, "   end_head: %d\n", printInfo_p->end_head);
			fprintf(stderr, "   end_sec: %d\n", printInfo_p->end_sec);
			fprintf(stderr, "   end_cyl: %d\n", printInfo_p->end_cyl);
			fprintf(stderr, "   lFirst: %d\n", printInfo_p->lFirst);
			fprintf(stderr, "   size: %d\n\n", printInfo_p->size);

		case INODE :
			printInfo_i = (struct inode*) printInfo_v;
			fprintf(stderr, "Subpartition contents:\nStoredFields:\n");
			fprintf(stderr, "   links: %d\n", printInfo_i->links);
			fprintf(stderr, "   uid: %d\n", printInfo_i->uid);
			fprintf(stderr, "   gid: %d\n", printInfo_i->gid);
			fprintf(stderr, "   size: %d\n", printInfo_i->size);
			fprintf(stderr, "   atime: %s\n", ctime(printInfo_i->atime));
			fprintf(stderr, "   mtime: %s\n", ctime(printInfo_i->mtime));
			fprintf(stderr, "   ctime: %s\n", ctime(printInfo_i->ctime));
			for (int i=0; i<DIRECT_ZONES; i++)
				fprintf(stderr, "   zone[%d]: %d\n", i, printInfo_i->zone[i]);

			fprintf(stderr, "   indirect: %d\n", printInfo_i->indirect);
			fprintf(stderr, "   two_indirect: %d\n", printInfo_i->two_indirect);
			fprintf(stderr, "   unused: %d\n", printInfo_i->unused);


		default :
			fprintf(stderr, "Error: invalid mode given to print verbose...\n");
			exit(EXIT_FAILURE);
	}

	return 0;
}