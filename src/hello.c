#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/inotify.h>

#define breakpoint() raise(SIGABRT)


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
handle_events(int fd, int * wds)
{
}


int
main(int argc, const char * args[])
{
  printf("HELLO WORLD\n");
  printf("Got %d args:\n", argc);
  for (int ii=0; ii<argc; ii++) {
    printf("  [%d]: %s\n", ii, args[ii]);
  }

  FILE * output = popen("ls", "r");
  size_t size_buff = 512;
  char buff[size_buff+1];

  size_t size_read = fread(buff, 1, size_buff, output);
  buff[size_read] = '\0';

  printf("read %zu bytes: %s\n", size_read, buff);

//  printf("First %zu chars of output: %s\n", size_buff, buff);
  pclose(output);

  DIR * dir = opendir(".");
  struct dirent * dirent = readdir(dir);
  for(;;dirent = readdir(dir)) {
    if (dirent == NULL) {
      break;
    }
    if (dirent->d_name[0] == '.') {
      continue;
    }
    printf("name: %s\n", dirent->d_name);
  }

  closedir(dir);

  /* Try some inotify stuff. */
  int fd_inotify = inotify_init();

//  uint32_t mask_event = IN_OPEN | IN_CLOSE | IN_ACCESS | IN_MODIFY;
  uint32_t mask_event = IN_CLOSE_WRITE;

  inotify_add_watch(fd_inotify, ".", mask_event);
  inotify_add_watch(fd_inotify, "./src", mask_event);
  size_t size_inotify_event = sizeof(struct inotify_event);
  printf("Waiting for events.\n");
  for(;;) {
    size_read = read(fd_inotify, buff, size_buff);
//    printf("Past waiting for events, read: %zu bytes.\n", size_read);
    struct inotify_event * e = (struct inotify_event *)buff;
    for (int i=0; i < size_read/size_inotify_event; e++,i++) {
      // Only look at named events. 
      if (!e->len) {
        continue;
      }
      // Only changes in *.c files.
      if (!endswith(e->name, ".c")) {
        continue;
      }
      if (e->mask & IN_ISDIR) {
        printf("[Event #%d]: Directory event", i);
      } else {
        printf("[Event #%d]: File event", i);
      }
      if (e->mask & IN_CLOSE_WRITE) {
        printf(",IN_CLOSE_WRITE");
      }
      printf(",event->name: %s\n", e->name);
    }
  }
}
