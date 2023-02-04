#include <stdio.h>
#include <string.h>
#include "create.h"
#include "apply.h"

void usage()
{
	fprintf(stderr, "usage: blockdelta create <block1> <block2>\n");
	fprintf(stderr, "       blockdelta apply <block>\n");
}

int main (int argc, char *argv[])
{
	if (argc < 3) {
		usage();
		return -1;
	}

	if (strcmp(argv[1], "create") == 0) {
		if (argc < 4) {
			usage();
			return -1;
		}
		Create* create = new Create();
		int result = create->run(argv[2], argv[3]);
		delete create;
		return result;
	} else if (strcmp(argv[1], "apply") == 0) {
		Apply* apply = new Apply();
		int result = apply->run(argv[2]);
		delete apply;
		return result;
	} else {
		usage();
		return -1;
	}
}
