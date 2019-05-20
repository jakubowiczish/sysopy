#define _XOPEN_SOURCE 500
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "backup.h"
#include "error.h"

#define MONITOR_MAX 1024

static pid_t children[MONITOR_MAX];
static char* monitor_files[MONITOR_MAX];
static int monitor_intervals[MONITOR_MAX];
static int monitor_size = 0;
static int running = 1;
static int copies = 0;

void parse_list(const char* list_filename) {
  FILE* list_file = fopen(list_filename, "r");
  if (list_file == NULL) {
    perr("unable to open list file %s", list_filename);
  }

  while (1) {
    char* file_path = malloc(4096);
    int interval;

    int result = fscanf(list_file, "%s%d", file_path, &interval);

    if (result == EOF) {
      free(file_path);
      break;
    } else if (result != 2) {
      err("error while scanning the list file");
    }

    monitor_files[monitor_size] = file_path;
    monitor_intervals[monitor_size] = interval;
    ++monitor_size;
  }

  fclose(list_file);
}

void monitor_end(int sg) {
  exit(copies);
  printf("PID: %d exiting\n", getpid());
}

void monitor_start(int sg) {
  running = 1;
  printf("PID: %d running\n", getpid());
}

void monitor_stop(int sg) {
  running = 0;
  printf("PID: %d paused\n", getpid());
}

void monitor(int element) {
  struct sigaction sa_start, sa_stop, sa_end;
  memset(&sa_start, 0, sizeof(struct sigaction));
  memset(&sa_stop, 0, sizeof(struct sigaction));
  memset(&sa_end, 0, sizeof(struct sigaction));

  sa_start.sa_handler = monitor_start;
  sa_stop.sa_handler = monitor_stop;
  sa_end.sa_handler = monitor_end;

  sigaction(SIGUSR1, &sa_start, NULL);
  sigaction(SIGUSR2, &sa_stop, NULL);
  sigaction(SIGTERM, &sa_end, NULL);

  pid_t pid = getpid();
  char* filename = monitor_files[element];
  int interval = monitor_intervals[element];

  time_t modification;
  struct stat sb;

  if (lstat(filename, &sb) < 0) {
    fprintf(stderr, "PID: %d unable to lstat file %s\n", pid, filename);
    exit(0);
  }

  backup_mem_read(filename);

  modification = sb.st_mtime;

  while (1) {
    if (running) {
      if (lstat(filename, &sb) < 0) {
        fprintf(stderr, "PID: %d unable to lstat file %s\n", pid, filename);

        exit(0);
      }

      if (sb.st_mtime != modification) {
        modification = sb.st_mtime;
        backup_mem(filename);
        ++copies;
      }

      sleep(interval);
    } else {
      usleep(100000);
    }
  }
}

void end_main_process(int sg) {
  printf("\nterminating monitor...\n");
  for (int i = 0; i < monitor_size; ++i) {
    kill(children[i], SIGTERM);
    int status;
    waitpid(children[i], &status, 0);
    int copies = WEXITSTATUS(status);
    printf("PID: %d made %d copies\n", children[i], copies);
    free(monitor_files[i]);
  }

  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    err("usage: %s [list]", argv[0]);
  }

  char* list_filename = argv[1];

  struct stat sb;
  if (lstat("archive", &sb) < 0) {
    if (mkdir("archive", 0755) == -1) {
      perr("cannot create archive directory");
    }
  } else {
    if (!S_ISDIR(sb.st_mode)) {
      err("cannot create archive directory: file exists");
    }
  }

  parse_list(list_filename);

  for (int i = 0; i < monitor_size; ++i) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
      perr("unable to fork");
    } else if (child_pid > 0) {
      children[i] = child_pid;
      printf("PID: %d monitoring %s\n", child_pid, monitor_files[i]);
    } else {
      struct sigaction sa_ignore_ctrl_c;
      memset(&sa_ignore_ctrl_c, 0, sizeof(struct sigaction));
      sa_ignore_ctrl_c.sa_handler = SIG_IGN;
      sigaction(SIGINT, &sa_ignore_ctrl_c, NULL);
      monitor(i);
      _exit(EXIT_FAILURE);
    }
  }

  struct sigaction sa_end_main_process;
  memset(&sa_end_main_process, 0, sizeof(struct sigaction));
  sa_end_main_process.sa_handler = end_main_process;
  sigaction(SIGINT, &sa_end_main_process, NULL);

  char input[256];
  char delimiters[] = " \r\n\t";
  char* word;

  while (1) {
    fgets(input, sizeof(input), stdin);

    word = strtok(input, delimiters);
    if (word == NULL) {
      continue;
    }

    if (strcmp(word, "list") == 0) {
      for (int i = 0; i < monitor_size; ++i) {
        printf("PID: %d monitoring %s\n", children[i], monitor_files[i]);
      }
    } else if (strcmp(word, "end") == 0) {
      end_main_process(SIGINT);
    } else {
      int start = strcmp(word, "start");
      if (!(start == 0 || strcmp(word, "stop") == 0)) {
        printf("unknown command\n");
        continue;
      }

      word = strtok(NULL, delimiters);
      if (word == NULL) {
        printf("you have to provide either PID or 'all'\n");
      } else {
        if (strcmp(word, "all") == 0) {
          if (start == 0) {
            for (int i = 0; i < monitor_size; ++i) {
              kill(children[i], SIGUSR1);
            }
          } else {
            for (int i = 0; i < monitor_size; ++i) {
              kill(children[i], SIGUSR2);
            }
          }
        } else {
          char* end;
          pid_t pid = strtol(word, &end, 10);
          if (end == word) {
            printf("provided PID is not a valid number\n");
          } else {
            int found = 0;
            for (int i = 0; i < monitor_size; ++i) {
              if (children[i] == pid) {
                found = 1;
                break;
              }
            }
            if (found == 0) {
              printf("provided PID is not a monitor process\n");
            } else if (start == 0) {
              kill(pid, SIGUSR1);
            } else {
              kill(pid, SIGUSR2);
            }
          }
        }
      }
    }
  }

  return 0;
}
