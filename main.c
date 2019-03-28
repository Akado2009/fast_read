/* file stat lib */
#include <unistd.h>
#include <sys/stat.h>

/* error logging libs */
#include <string.h>
#include <errno.h>
#include <stdarg.h>

/* file libs */
#include <fcntl.h>
#include <stdio.h>

/* exit lib */
#include <stdlib.h>

/* memory-map library */
#include <sys/mman.h>

/* external shit */
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/* threading & preproccesing */
#include <pthread.h>


#define GB_SIZE 1024 * 1024 * 1024
unsigned long long int NO_CHUNKS, PAGE_SIZE, CHUNK_SIZE, TOTAL_SIZE, buffer, share, offset;
FILE *output;

static void check(int test, char *message, ...) {
	if (test) {
		va_list args;
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}
}

void define_no_pages(const char *filename) {
	struct stat s;
	stat(filename, &s);
	PAGE_SIZE = getpagesize();
	TOTAL_SIZE = s.st_size/PAGE_SIZE;
	CHUNK_SIZE = 1024*1024*1024*0.4/PAGE_SIZE;
	if (s.st_size % PAGE_SIZE != 0)
	TOTAL_SIZE++;
}

void mmap_chunk_and_preprocess(const char *filename) {
	int src, stat_code;
	char *sm;
	struct stat statbuf;

	src = open(filename, O_RDONLY);
	check(src < 0, "Failed to open a file: %s", strerror(errno));

	stat_code = fstat(src, &statbuf);
	check(stat_code < 0, "Failed to get file info: %s", strerror(errno));
	
	// printf("share->%lli PAGES per node\n",share);
	if (share >= CHUNK_SIZE)
		buffer = CHUNK_SIZE;
	else 
		buffer = share;
	/*
	printf("total pieces->%lli\n",TOTAL_SIZE);
	printf("data left->%lli\n",share);
	printf("buffer size->%lli\n",buffer);
	printf("PAGE size->%lli\n",PAGE_SIZE);
	*/
	sm = mmap(0, buffer*PAGE_SIZE, PROT_READ, MAP_SHARED | MAP_NORESERVE, src, offset*PAGE_SIZE);
	check(sm == MAP_FAILED, "Failed to mmap file: %s", strerror(errno));
	long previous_i = 0;
	long extra_space = 20; // extra space for extra columns
	for(long i = 0; i < buffer*PAGE_SIZE; ++i) {
		if (sm[i] == '\n') {
			char *result = (char *)malloc((i - previous_i + extra_space) * sizeof(char));
			strncpy(result, sm + previous_i, i - previous_i);
			printf("%s\n", result);
			previous_i = i + 1;
		}
	}
	munmap(sm, buffer * PAGE_SIZE);
}

int main() {
	int fd;
	struct stat s;
	int status;
	size_t size;
	
	const char *filename = "sample.csv";
	const char *mapped;
	
	const char *output_filename = "results.txt";
	output = fopen(output_filename, "w");
	/*
	fd = open(filename, O_RDONLY);
	check(fd < 0, "Failed to open file %s, error: %s", filename, strerror(errno));

	status = fstat(fd, &s);
	check(status < 0, "Failed to check status: %s", strerror(errno));

	size = s.st_size;
	printf("Size of the file is %lu\n", size);
	
	mapped = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
	check(mapped == MAP_FAILED, "Mmap failed: %s", strerror(errno));
	*/
	
	/* GET SIZE */
	/* DEFINE A CHUNK SIZE */
	define_no_pages(filename);
	/*
	printf("PAGE_SIZE\tTOTAL_SIZE\tCHUNK_SIZE\tGB_SIZE\n");
	printf("%llu\t%llu\t%llu\t%i\n", PAGE_SIZE, TOTAL_SIZE, CHUNK_SIZE, GB_SIZE);
	*/
	/* NO_CHUNKS = NO_THREADS */
	/* PARSE CHUNKS IN EACH THREAD */
	share = TOTAL_SIZE;
	offset = 0;

	clock_t start = clock();

	while (share > 0) {
		mmap_chunk_and_preprocess(filename);
		share -= buffer;
		offset += buffer;
	}

	clock_t stop = clock();
	printf("%f sec elapsed\n", (double)(stop - start)/CLOCKS_PER_SEC);
	return 0;	

}
