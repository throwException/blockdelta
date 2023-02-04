#include <cstdint>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "apply.h"
#include "config.h"
#include "util.h"

int Apply::run(char* blockpath)
{
	int fd = open(blockpath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Cannot open file %s\n", blockpath);
		return -2;
	}

	FILE* block = fdopen(fd, "w");
	if (block == nullptr) {
		close(fd);
		fprintf(stderr, "Cannot open file %s\n", blockpath);
		return -2;
	}

	bool success = true;
	uint64_t block_number;
	uint8_t buffer[BLOCK_SIZE];

	while (true) {
		if (fread(&block_number, sizeof(uint64_t), 1, stdin) != 1) {
			break;
		}
		if (fread(buffer, BLOCK_SIZE, 1, stdin) != 1) {
			break;
		}
		block_number = host_to_big_endian(block_number); 
		if (fseek(block, block_number * BLOCK_SIZE, SEEK_SET) != 0) {
			success = false;
			break;
		}
		if (fwrite(buffer, BLOCK_SIZE, 1, block) != 1) {
			success = false;
			break;
		}
	}

	if (ferror(stdin) != 0) {
		success = false;
	}

	fclose(block);

	if (success) {
		return 0;
	} else {
		fprintf(stderr, "I/O error\n");
		return -5;
	}
}

