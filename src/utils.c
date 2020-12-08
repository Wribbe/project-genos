#include "utils.h"


//size_t size_buff = 512;
//char buff[size_buff+1];
//
///* Try some inotify stuff. */
//int fd_inotify = inotify_init();
//
//uint32_t mask_event = IN_CLOSE_WRITE;
//
//inotify_add_watch(fd_inotify, ".", mask_event);
//inotify_add_watch(fd_inotify, "./src", mask_event);
//size_t size_inotify_event = sizeof(struct inotify_event);
//printf("Waiting for events.\n");
//for(;;) {
//  size_read = read(fd_inotify, buff, size_buff);
//  struct inotify_event * e = (struct inotify_event *)buff;
//  for (int i=0; i < size_read/size_inotify_event; e++,i++) {
//    // Only look at named events. 
//    if (!e->len) {
//      continue;
//    }
//    // Only changes in *.c files.
//    if (!endswith(e->name, ".c")) {
//      continue;
//    }
//    if (e->mask & IN_ISDIR) {
//      printf("[Event #%d]: Directory event", i);
//    } else {
//      printf("[Event #%d]: File event", i);
//    }
//    if (e->mask & IN_CLOSE_WRITE) {
//      printf(",IN_CLOSE_WRITE");
//    }
//    printf(",event->name: %s\n", e->name);
//  }
//}



bool
endswith(const char * str, const char * ending)
{
  size_t lenstr = strlen(str);
  size_t lenending = strlen(ending);
  for (size_t ii=1; ii<=lenending; ii++) {
    if (str[lenstr-ii] != ending[lenending-ii]) {
      return false;
    }
  }
  return true;
}


void 
sig_handler_empty(int sig){}

void *
worker_watcher(void * args)
{
  struct watcher * watcher = (struct watcher *)args; 

  struct sigaction sa = {0};
  sa.sa_handler = sig_handler_empty; 
  sigemptyset(&sa.sa_mask);
  int res = sigaction(SIGUSR1, &sa, NULL);
//  printf("Result of sigaction: %d\n", res);

  for(;;) {
    pthread_mutex_lock(&watcher->mutex_modifications);
    if (watcher->command_stop) {
      break;
    }
    pthread_mutex_unlock(&watcher->mutex_modifications);
//    printf("Reading from event queue.\n");
    read(watcher->fd_inotify, watcher->buffer_events, SIZE_BUFFER_EVENTS);
//    printf("After read.\n");
  }
  pthread_mutex_unlock(&watcher->mutex_modifications);
//  printf("Got stop command, aborting.\n");
  return (void *)-1;
}


struct watcher *
watcher_init(void) 
{
  struct watcher * watcher = malloc(sizeof(struct watcher));
  watcher->fd_inotify = inotify_init();
  watcher->status_events_new = false;
  watcher->command_stop = false;
  pthread_mutex_init(&watcher->mutex_modifications, NULL);

  pthread_create(&watcher->thread_watcher, NULL, worker_watcher, watcher);

  return watcher;
}


int
watcher_add(struct watcher * watcher, const char * path)
{
  return inotify_add_watch(watcher->fd_inotify, path, IN_CLOSE_WRITE);
}
