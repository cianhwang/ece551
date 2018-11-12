#include <assert.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define tableSize 65521
bool isSymlink(std::string filename) {
  struct stat sb;
  lstat(filename.c_str(), &sb);
  if ((sb.st_mode & S_IFMT) == S_IFLNK) {
    return true;
  }
  return false;
}

bool isDirectory(std::string filename) {
  struct stat sb;
  lstat(filename.c_str(), &sb);
  if ((sb.st_mode & S_IFMT) == S_IFDIR) {
    return true;
  }
  return false;
}

bool compContent(std::string filename, std::string dupname);
void putToShell(std::string filename, std::string dupname);

void fileVec(std::string homeroot, std::vector<std::string> * hashTable) {
  char * p1 = realpath(homeroot.c_str(), NULL);
  homeroot = p1;
  free(p1);
  if (!isDirectory(homeroot)) {
    return;
  }
  DIR * dirp;
  struct dirent * directory;
  dirp = opendir(homeroot.c_str());
  if (dirp != NULL) {
    while ((directory = readdir(dirp)) != NULL) {
      std::string filename(directory->d_name);
      if (filename == "." || filename == ".." || isSymlink(homeroot + "/" + filename)) {
        continue;
      }
      else {
        char * p2 = realpath((homeroot + "/" + filename).c_str(), NULL);

        std::string absPath(p2);

        free(p2);
        if (isDirectory(absPath)) {
          fileVec(absPath, hashTable);
        }
        else {
          std::string filename = absPath;
          std::ifstream t(filename);
          std::stringstream buffer;
          buffer << t.rdbuf();
          std::hash<std::string> hashStr;
          int idx = hashStr(buffer.str()) % tableSize;

          bool dup = false;
          if (hashTable[idx].size() != 0) {  //comparison
            // if same content, output to sh, continue;
            for (auto const & value : hashTable[idx]) {
              if (compContent(filename, value)) {
                putToShell(filename, value);

                dup = true;
                break;
              }
            }
          }
          if (!dup) {
            hashTable[idx].push_back(filename);
          }
        }
      }
    }
    closedir(dirp);
  }
}

bool compContent(std::string filename, std::string dupname) {
  int result = system(("diff " + filename + " " + dupname + " > /dev/null 2>&1").c_str());
  if (filename != dupname && result == 0) {
    return true;
  }
  return false;
}

void putToShell(std::string filename, std::string dupname) {
  std::cout << "#Removing " << filename << " ";
  std::cout << "(duplicate of " << dupname << ").\n\n";
  std::cout << "rm " << filename << "\n\n";
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    perror("Please input directory name.");
    return EXIT_FAILURE;
  }
  std::cout << "#!/bin/bash\n\n";
  std::vector<std::string> hashTable[tableSize];

  for (int i = 1; i < argc; ++i) {
    std::string root(argv[i]);
    fileVec(root, hashTable);
  }
  return EXIT_SUCCESS;
}
