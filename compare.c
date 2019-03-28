#include <stdio.h>
#include <time.h>
#include <ctype.h>


int main() {
	clock_t start = clock();
	unsigned long long int count = 0;
	FILE *input_file;
	input_file = fopen("sample.csv", "r");
	char c;
	while (c = fgetc(input_file) != EOF) {
		if (!isalpha(c) && !isspace(c)) count++;
	}
	clock_t stop = clock();
	printf("%f\n", (double)(stop - start)/CLOCKS_PER_SEC);
	return 0;
}
