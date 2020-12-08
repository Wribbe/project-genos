#include "utils.h"

int
main(int argc, const char * args[])
{
  struct watcher * watcher = watcher_init();
  watcher_add(watcher, "./src");
  
  size_t size_buff = 100;
  char buff[size_buff];
  for(;;) {
    fgets(buff, size_buff, stdin);
    if (buff[0] == 'q') {
      printf("Quitting.\n");
      break;
    } else if (buff[0] == 'k') {
      printf("Getting modification mutex.\n");
      pthread_mutex_lock(&watcher->mutex_modifications);
      printf("Got modification mutex.\n");
      printf("Setting command_stop.\n");
      watcher->command_stop = true;
      printf("Releasing modification mutex.\n");
      pthread_mutex_unlock(&watcher->mutex_modifications);
      printf("Released modification mutex.\n");
      pthread_kill(watcher->thread_watcher, SIGUSR1);
    } else {
      printf("Doing nothing.\n");
      printf("Submit 'k' for thread-kill, Submit 'q' to exit.\n");
    }
  }
}
