#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "error.h"

int main(int argc, char* argv[]) {
  if (argc != 5) {
    err("usage: %s [file] [interval_min] [interval_max] [bytes]", argv[0]);
  }

  char* filename = argv[1];

  char* end;
  int interval_min = strtol(argv[2], &end, 10);
  if (end == argv[2]) {
    err("cannot convert '%s' to a number", argv[2]);
  }

  int interval_max = strtol(argv[3], &end, 10);
  if (end == argv[3]) {
    err("cannot convert '%s' to a number", argv[3]);
  }

  int bytes = strtol(argv[4], &end, 10);
  if (end == argv[4]) {
    err("cannot convert '%s' to a number", argv[4]);
  }

  if (interval_min > interval_max) {
    err("interval_min should be lower than interval_max");
  }

  srand(time(NULL));

  FILE* f;
  char* random_buffer = malloc(bytes);
  time_t rawtime;
  struct tm* timeinfo;
  char date[80];

  while (1) {
    int sleep_interval =
        interval_min + (rand() % (interval_max - interval_min + 1));

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", timeinfo);

    f = fopen("/dev/urandom", "rb");
    fread(random_buffer, bytes, 1, f);
    fclose(f);

    f = fopen(filename, "a");
    fprintf(f, "pid: %d sleep: %d date: %s\nbytes: ", getpid(), sleep_interval,
            date);
    fwrite(random_buffer, bytes, 1, f);
    fputs("\n\n", f);
    fclose(f);

    sleep(sleep_interval);
  }

  free(random_buffer);
  return 0;
}
