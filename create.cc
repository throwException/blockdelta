#include <cstdint>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "create.h"
#include "config.h"
#include "util.h"

int Create::run(char* block1path, char* block2path)
{
	FILE* block1 = fopen(block1path, "r");
	if (block1 == nullptr) {
		fprintf(stderr, "Cannot open file %s\n", block1path);
		return -2;
	}

	FILE* block2 = fopen(block2path, "r"); 
	if (block2 == nullptr) {
		fclose(block1);
		fprintf(stderr, "Cannot open file %s\n", block2path);
		return -2;
	}

	uint64_t block1size = file_size(block1path);
	uint64_t block2size = file_size(block2path);

	if (block1size != block2size) {
		fprintf(stderr, "File sizes are not equal\n");
		fclose(block1);
		fclose(block2);
		return -3;
	}
	if (block1size % BLOCK_SIZE != 0) {
		fprintf(stderr, "File sizes are not divisible by block size %u\n", BLOCK_SIZE);
		fclose(block1);
		fclose(block2);
		return -4;
	}

	uint64_t block_count = block1size / BLOCK_SIZE;
	char buffer1[BLOCK_SIZE];
	char buffer2[BLOCK_SIZE];
	bool success = true;

	for (uint64_t i = 0; i < block_count; i++) {
		if (fread(buffer1, BLOCK_SIZE, 1, block1) != 1) {
			success = false;
			break;
		}
		if (fread(buffer2, BLOCK_SIZE, 1, block2) != 1) {
			success = false;
			break;
		}
		if (memcmp(buffer1, buffer2, BLOCK_SIZE) != 0) {
			uint64_t block_number = host_to_big_endian(i);
			if (fwrite(&block_number, sizeof(uint64_t), 1, stdout) != 1) {
				success = false;
				break;
			}
			if (fwrite(buffer2, BLOCK_SIZE, 1, stdout) != 1) {
				success = false;
				break;
			}
		}
		if (i % 256 == 0) {
			fprintf(stderr, "%llu/%llu\n", i, block_count);
		}
	}

	fclose(block1);
	fclose(block2);
	fclose(stdout);

	if (success) {
		return 0;
	} else {
		fprintf(stderr, "I/O error\n");
		return -5;
	}
}

