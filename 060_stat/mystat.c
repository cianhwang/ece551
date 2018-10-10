#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}

void idFileType(struct stat sb, char * s) {
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      strcpy(s, "block special file");
      break;
    case S_IFCHR:
      strcpy(s, "character special file");
      break;
    case S_IFDIR:
      strcpy(s, "directory");
      break;
    case S_IFIFO:
      strcpy(s, "fifo");
      break;
    case S_IFLNK:
      strcpy(s, "symbolic link");
      break;
    case S_IFREG:
      strcpy(s, "regular file");
      break;
    case S_IFSOCK:
      strcpy(s, "socket");
      break;
    default:
      break;
  }
}

void idFileReadable(struct stat sb, char * s) {
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      s[0] = 'b';
      break;
    case S_IFCHR:
      s[0] = 'c';
      break;
    case S_IFDIR:
      s[0] = 'd';
      break;
    case S_IFIFO:
      s[0] = 'p';
      break;
    case S_IFLNK:
      s[0] = 'l';
      break;
    case S_IFREG:
      s[0] = '-';
      break;
    case S_IFSOCK:
      s[0] = 's';
      break;
    default:
      break;
  }
  s[1] = ((sb.st_mode & S_IRUSR) == 0) ? '-' : 'r';
  s[2] = ((sb.st_mode & S_IWUSR) == 0) ? '-' : 'w';
  s[3] = ((sb.st_mode & S_IXUSR) == 0) ? '-' : 'x';
  s[4] = ((sb.st_mode & S_IRGRP) == 0) ? '-' : 'r';
  s[5] = ((sb.st_mode & S_IWGRP) == 0) ? '-' : 'w';
  s[6] = ((sb.st_mode & S_IXGRP) == 0) ? '-' : 'x';
  s[7] = ((sb.st_mode & S_IROTH) == 0) ? '-' : 'r';
  s[8] = ((sb.st_mode & S_IWOTH) == 0) ? '-' : 'w';
  s[9] = ((sb.st_mode & S_IXOTH) == 0) ? '-' : 'x';
  s[10] = '\0';
}

int main(int argc, char ** argv) {
  struct stat sb;
  if (argc != 2) {
    fprintf(stderr, "Wrong para.");
    return EXIT_FAILURE;
  }
  printf("  File: ‘%s’\n", argv[1]);

  if (lstat(argv[1], &sb) == -1) {
    perror("...");
    return EXIT_FAILURE;
  }
  char * s = malloc(50 * sizeof(*s));
  idFileType(sb, s);
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         sb.st_size,
         sb.st_blocks,
         sb.st_blksize,
         s);

  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
         (long)major(sb.st_dev),
         (long)minor(sb.st_dev),
         sb.st_ino,
         sb.st_nlink);
  s = realloc(s, 11 * sizeof(*s));
  idFileReadable(sb, s);
  printf("Access: (%04o/%s)\n", sb.st_mode & ~S_IFMT, s);
  free(s);
}
