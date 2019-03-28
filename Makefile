CC=gcc
CFLAGS=-O3 -Wall
LIBS=-pthread
RM=rm -f 
BINARIES=preprocessor compare time_test

build:
	$(CC) $(CFLAGS) $(LIBS) -o preprocessor main.c
compare:
	$(CC) $(CLFAGS) $(LIBS) -o compare compare.c
time:
	$(CC) $(CFLAGS) $(LIBS) -o time_test time_test.c
clean:
	$(RM) $(BINARIES)
