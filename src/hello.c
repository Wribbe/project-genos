#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <sys/inotify.h>


int
main(int argc, const char * args[])
{
  printf("HELLO WORLD\n");
  printf("Got %d args:\n", argc);
  for (int ii=0; ii<argc; ii++) {
    printf("  [%d]: %s\n", ii, args[ii]);
  }

  FILE * output = popen("ls", "r");
  size_t size_buff = 200;
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

  inotify_add_watch( fd_inotify, ".", 
      IN_OPEN | IN_CLOSE | IN_ACCESS | IN_MODIFY
  );
  printf("Waiting for events.\n");
  size_read = read(fd_inotify, buff, size_buff);
  printf("Past waiting for events, read: %zu bytes.\n", size_read);
}
