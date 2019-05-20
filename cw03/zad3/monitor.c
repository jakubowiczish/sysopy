#define _XOPEN_SOURCE 500
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "backup.h"
#include "error.h"

#define MONITOR_MAX 1024

static char* monitor_files[MONITOR_MAX];
static int monitor_intervals[MONITOR_MAX];
static int monitor_size = 0;

#define BACKUP_MEM 0
#define BACKUP_EXEC 1
static int backup_type;

static struct rlimit cpu_rlimit, mem_rlimit;

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

void monitor(int element) {
  if (setrlimit(RLIMIT_CPU, &cpu_rlimit) < 0) {
    perr("unable to set cpu limit");
  }
  if (setrlimit(RLIMIT_AS, &mem_rlimit) < 0) {
    perr("unable to set mem limit");
  }

  pid_t pid = getpid();
  char* filename = monitor_files[element];
  int interval = monitor_intervals[element];

  time_t modification;
  struct stat sb;

  if (lstat(filename, &sb) < 0) {
    fprintf(stderr, "PID: %d unable to lstat file %s\n", pid, filename);
    exit(0);
  }

  if (backup_type == BACKUP_EXEC) {
    backup_exec(filename);
  } else if (backup_type == BACKUP_MEM) {
    backup_mem_read(filename);
  }

  modification = sb.st_mtime;

  printf("PID: %d monitoring %s every %d seconds\n", pid, filename, interval);

  char* quit_event;

  int shmid;
  if ((shmid = shmget(1337, 1, IPC_CREAT | 0666)) < 0) {
    fprintf(stderr, "PID: %d unable to create shared memory\n", pid);
    exit(0);
  }

  if ((quit_event = shmat(shmid, NULL, 0)) == (char*)-1) {
    fprintf(stderr, "PID: %d unable to attach shared memory\n", pid);
    exit(0);
  }

  int copies = 0;
  while (*quit_event != 'y') {
    if (lstat(filename, &sb) < 0) {
      fprintf(stderr, "PID: %d unable to lstat file %s\n", pid, filename);
      exit(0);
    }

    if (sb.st_mtime != modification) {
      modification = sb.st_mtime;
      if (backup_type == BACKUP_EXEC) {
        backup_exec(filename);
      } else if (backup_type == BACKUP_MEM) {
        backup_mem(filename);
      }

      ++copies;
    }

    sleep(interval);
  }

  struct rusage rusage;
  double user_time, system_time;

  getrusage(RUSAGE_SELF, &rusage);
  user_time =
      rusage.ru_utime.tv_sec * 1.0 + rusage.ru_utime.tv_usec / 1000000.0;
  system_time =
      rusage.ru_stime.tv_sec * 1.0 + rusage.ru_stime.tv_usec / 1000000.0;
  printf("PID: %d SELF    \tu_time: %.6fs  s_time: %.6fs  maxrss: %ld\n", pid,
         user_time, system_time, rusage.ru_maxrss);

  getrusage(RUSAGE_CHILDREN, &rusage);
  user_time =
      rusage.ru_utime.tv_sec * 1.0 + rusage.ru_utime.tv_usec / 1000000.0;
  system_time =
      rusage.ru_stime.tv_sec * 1.0 + rusage.ru_stime.tv_usec / 1000000.0;
  printf("PID: %d CHILDREN\tu_time: %.6fs  s_time: %.6fs  maxrss: %ld\n", pid,
         user_time, system_time, rusage.ru_maxrss);

  exit(copies);
}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    err("usage: %s [list] [timeout] [mode] [cpu_limit] [mem_limit]", argv[0]);
  }

  char* list_filename = argv[1];
  char* backup_mode = argv[3];
  if (strcmp(backup_mode, "mem") == 0) {
    backup_type = BACKUP_MEM;
  } else if (strcmp(backup_mode, "exec") == 0) {
    backup_type = BACKUP_EXEC;
  } else {
    err("valid backup modes: mem, exec");
  }

  char* end;
  int timeout = strtol(argv[2], &end, 10);
  if (end == argv[2]) {
    err("cannot convert '%s' to a number", argv[2]);
  }

  int cpu_limit = strtol(argv[4], &end, 10);
  if (end == argv[4]) {
    err("cannot convert '%s' to a number", argv[4]);
  }

  int mem_limit = strtol(argv[5], &end, 10);
  if (end == argv[5]) {
    err("cannot convert '%s' to a number", argv[5]);
  }

  cpu_rlimit.rlim_cur = cpu_limit;
  cpu_rlimit.rlim_max = cpu_limit;
  mem_rlimit.rlim_cur = mem_limit * 1048576;
  mem_rlimit.rlim_max = mem_limit * 1048576;

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

  char* quit_event;

  int shmid;
  if ((shmid = shmget(1337, 1, IPC_CREAT | 0666)) < 0) {
    perr("unable to create shared memory");
  }

  if ((quit_event = shmat(shmid, NULL, 0)) == (char*)-1) {
    perr("unable to attach shared memory");
  }

  *quit_event = 'n';

  pid_t children[MONITOR_MAX];
  for (int i = 0; i < monitor_size; ++i) {
    pid_t child_pid = fork();

    if (child_pid == -1) {
      perr("unable to fork");
    } else if (child_pid > 0) {
      children[i] = child_pid;
    } else {
      monitor(i);
      _exit(EXIT_FAILURE);
    }
  }

  sleep(timeout);
  *quit_event = 'y';
  for (int i = 0; i < monitor_size; ++i) {
    int status;
    waitpid(children[i], &status, 0);
    int copies = WEXITSTATUS(status);
    printf("PID: %d made %d copies\n", children[i], copies);
    free(monitor_files[i]);
  }

  return 0;
}
