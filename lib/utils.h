#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>

#include <sys/inotify.h>
#include <pthread.h>

#define SIZE_BUFFER_EVENTS 256

#define breakpoint() raise(SIGABRT)

struct watcher {
  bool status_events_new;
  bool command_stop;
  int fd_inotify;
  pthread_mutex_t mutex_modifications;
  pthread_t thread_watcher;
  char * buffer_events[SIZE_BUFFER_EVENTS];
};

bool
endswith(const char * str, const char * ending);

struct watcher *
watcher_init(void);

int
watcher_add(struct watcher * watcher, const char * path);

#endif
