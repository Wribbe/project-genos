#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <dirent.h>


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

  size_t read = fread(buff, 1, size_buff, output);
  buff[read] = '\0';

  printf("read %zu bytes: %s\n", read, buff);

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

}
