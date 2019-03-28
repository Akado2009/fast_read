#include <stdio.h>
#include <time.h>


int main() {
	const char *raw_time = "2018-01-01 00:21:05";
	struct tm tm;
	strptime(raw_time, "%Y-%m-%d %H:%M:%S", &tm);
	printf("year: %d; month: %d; day: %d;\n",
	        tm.tm_year, tm.tm_mon, tm.tm_mday);
	printf("hour: %d; minute: %d; second: %d\n",
		        tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("week day: %d; year day: %d\n", tm.tm_wday, tm.tm_yday);
	return 0;
}
