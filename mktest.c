/*
 * Copyright (C) 2011 Andrea Mazzoleni
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "portable.h"

/**
 * Pseudo random number generator.
 */
unsigned long long seed = 0;

unsigned rnd(unsigned max) 
{
	seed = seed * 6364136223846793005LL + 1442695040888963407LL;

	return (seed >> 32) % max;
}

char CHARSET[] = "qwertyuiopasdfghjklzxcvbnm1234567890 .-+";
#define CHARSET_LEN (sizeof(CHARSET)-1)

void generate(int disk, int size)
{
	char path[PATH_MAX];
	FILE* f;
	char* file;
	int count;
	int i;
	int l;

	snprintf(path, sizeof(path), "test/disk%d/", disk);
	i = strlen(path);   

	/* add a directory */
	if (rnd(2) == 0) {
		path[i++] = '_';
		path[i++] = 'a' + rnd(2);
		path[i] = 0;

		/* create it */
		if (mkdir(path, 0777) != 0) {
			if (errno != EEXIST) {
				fprintf(stderr, "Error creating directory %s\n", path);
				exit(EXIT_FAILURE);
			}
		}

		path[i++] = '/';
	}

	/* add another directory */
	if (rnd(2) == 0) {
		path[i++] = '_';
		path[i++] = 'a' + rnd(2);
		path[i] = 0;

		/* create it */
		if (mkdir(path, 0777) != 0) {
			if (errno != EEXIST) {
				fprintf(stderr, "Error creating directory %s\n", path);
				exit(EXIT_FAILURE);
			}
		}

		path[i++] = '/';
	}
   
	/* add a file */
	l = 1 + rnd(20);
	file = path + i;
	while (l) {
		path[i++] = CHARSET[rnd(CHARSET_LEN)];
		--l;
	}
	path[i] = 0;

	/* skip some invalid file name, see http://en.wikipedia.org/wiki/Filename */
	if (strcmp(file, ".") == 0
		|| strcmp(file, "..") == 0
		|| strcmp(file, "prn") == 0
		|| strcmp(file, "con") == 0
		|| strcmp(file, "nul") == 0
		|| strcmp(file, "aux") == 0
		|| file[0] == ' '
		|| file[strlen(file)-1] == ' '
		|| file[strlen(file)-1] == '.'
	) {
		return;
	}

	f = fopen(path, "wb");
	if (!f) {
		fprintf(stderr, "Error writing %s\n", path);
		exit(EXIT_FAILURE);
	}

	count = size;
	while (count) {
		fputc(rnd(256), f);
		--count;
	}

	fclose(f);
}

void damage(const char* path, int size)
{
	FILE* f;
	struct stat st;
	off_t start;
	int count;

	f = fopen(path, "r+b");
	if (!f) {
		fprintf(stderr, "Error writing %s\n", path);
		exit(EXIT_FAILURE);
	}

	if (fstat(fileno(f), &st) != 0) {
		fprintf(stderr, "Error accessing %s\n", path);
		exit(EXIT_FAILURE);
	}

	/* start at random position inside the file */
	start = rnd(st.st_size);
	if (fseek(f, start, SEEK_SET) != 0) {
		fprintf(stderr, "Error seeking %s\n", path);
		exit(EXIT_FAILURE);
	}

	/* write garbage, in case also over the end */
	count = rnd(size);
	while (count) {
		fputc(rnd(256), f);
		--count;
	}

	fclose(f);
}

void help(void)
{
	printf("Test for " PACKAGE " v" VERSION " by Andrea Mazzoleni, " PACKAGE_URL "\n");
	printf("Usage:\n");
	printf("\tmktest generate SEED DISK_NUM FILE_NUM FILE_SIZE\n");
	printf("\tmktest damage SEED FAIL_NUM FAIL_SIZE FILE\n");
}

int main(int argc, char* argv[])
{
	int i, j;

	if (argc < 2) {
		help();
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "generate") == 0) {
		int disk, file, size;

		if (argc != 6) {
			help();
			exit(EXIT_FAILURE);
		}

		seed = atoi(argv[2]);
		disk = atoi(argv[3]);
		file = atoi(argv[4]);
		size = atoi(argv[5]);

		for(i=0;i<disk;++i) {
			for(j=0;j<file;++j) {
				if (j == 0)
					generate(i+1, size);
				else if (j == 1)
					generate(i+1, 0);
				else
					generate(i+1, rnd(size));
			}
		}
	} else if (strcmp(argv[1], "damage") == 0) {
		int fail, size;

		if (argc < 6) {
			help();
			exit(EXIT_FAILURE);
		}

		seed = atoi(argv[2]);
		fail = atoi(argv[3]);
		size = atoi(argv[4]);

		for(i=5;i<argc;++i) {
			for(j=0;j<fail;++j) {
				damage(argv[i], rnd(size));
			}
		}
	} else {
		help();
		exit(EXIT_FAILURE);
	}

	return 0;
}
