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

	int return_value = 0;
	uint64_t block_number;
	uint8_t buffer[BLOCK_SIZE];

	while (true) {
		if (fread(&block_number, sizeof(uint64_t), 1, stdin) != 1) {
			if (feof(stdin)) {
				break;
			} else {
				fprintf(stderr, "Error reading stdin\n");
				return_value = -5;
				break;
			}
		}
		if (fread(buffer, BLOCK_SIZE, 1, stdin) != 1) {
			if (feof(stdin)) {
				fprintf(stderr, "Unexpected EOF at stdin\n");
				return_value = -6;
				break;
			} else {
				fprintf(stderr, "Error reading stdin\n");
				return_value = -7;
				break;
			}
		}
		block_number = host_to_big_endian(block_number); 
		if (fseek(block, block_number * BLOCK_SIZE, SEEK_SET) != 0) {
			fprintf(stderr, "Error seeking position in block file\n");
			return_value = -7;
			break;
		}
		if (fwrite(buffer, BLOCK_SIZE, 1, block) != 1) {
			fprintf(stderr, "Error writing to block file\n");
			return_value = -8;
			break;
		}
	}

	if ((return_value == 0) &&
	    (ferror(stdin) != 0))
	{
		fprintf(stderr, "I/O error at stdin\n");
		return_value = -9;
	}

	if ((return_value == 0) &&
	    (ferror(block) != 0))
	{
		fprintf(stderr, "I/O error at block file\n");
		return_value = -10;
	}

	fclose(block);

	return return_value;
}

