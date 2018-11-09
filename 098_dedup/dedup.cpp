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

void fileVec(std::string homeroot, std::vector<std::string> & vec) {
  char * p1 = realpath(homeroot.c_str(), NULL);
  homeroot = p1;
  //  std::cout << homeroot << std::endl;
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
        //std::cout << filename.c_str() << std::endl;
        char * p2 = realpath((homeroot + "/" + filename).c_str(), NULL);
        assert(p2 != NULL);
        std::string absPath(p2);

        free(p2);
        if (isDirectory(absPath)) {
          fileVec(absPath, vec);
        }
        else {
          vec.push_back(absPath);
        }
      }
    }
    closedir(dirp);
  }
}

bool compContent(std::string filename, std::string dupname) {
  //  std::cout << "fun compContent: filename: " << filename << " "
  //      << "dupname: " << dupname << std::endl;
  /*  std::ifstream fs1(filename);
  std::ifstream fs2(filename);
  std::stringstream buf1;
  std::stringstream buf2;
  buf1 << fs1.rdbuf();
  buf2 << fs2.rdbuf();
  //  std::cout << buf1.str() << buf2.str();
  if (filename != dupname && buf1.str() == buf2.str()) {
    // std::cout << "TRUE!" << std::endl;
    return true;
  }
  return false;*/
  int result = system(("diff " + filename + " " + dupname).c_str());
  if (filename != dupname && result == 0) {
    return true;
  }
  return false;
}

void putToShell(std::string filename, std::string dupname) {
  //  std::cout << "fun puttoshell" << std::endl;
  std::fstream sh;
  sh.open("shell.sh", std::fstream::app);
  if (!sh.is_open()) {
    exit(EXIT_FAILURE);
  }

  //  char * path1 = realpath(filename.c_str(), NULL);
  //char * path2 = realpath(dupname.c_str(), NULL);
  sh << "#Removing " << filename << " ";
  sh << "(duplicate of " << dupname << ").\n\n";
  sh << "rm " << filename << std::endl;
  sh.close();
  //  free(path1);
  //  free(path2);
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    perror("Please input directory name.");
    return EXIT_FAILURE;
  }
  std::ofstream sh;
  sh.open("shell.sh");
  if (!sh.is_open()) {
    exit(EXIT_FAILURE);
  }
  sh << "#!/bin/bash\n";
  sh.close();

  std::vector<std::string> hashTable[97];
  std::vector<std::string> vec;
  for (int i = 1; i < argc; ++i) {
    std::string root(argv[i]);
    fileVec(root, vec);
    //   std::cout << "-finish.\n";
  }
  for (unsigned i = 0; i < vec.size(); ++i) {
    std::string filename = vec[i];
    //    std::cout << filename << std::endl;
    std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();
    //    std::cout << buffer.str();
    std::hash<std::string> hashStr;
    int hashNum = 97;
    int idx = hashStr(buffer.str()) % hashNum;
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
    //push_back
    if (!dup) {
      hashTable[idx].push_back(filename);
    }
  }
  return EXIT_SUCCESS;
}
