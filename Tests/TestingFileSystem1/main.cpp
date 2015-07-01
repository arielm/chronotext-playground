#include "Platform.h"

#include <iostream>

#include "boost/filesystem/fstream.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  auto executablePath = chr::getExecutablePath(argc, argv);
  auto filePath = chr::getResourcePath(executablePath, "test.txt");

  fs::ifstream in(filePath, ios::in | ios::binary | ios::ate);
  
  if (in)
  {
    auto fileSize = in.tellg();
    in.seekg(0, ios::beg);

    string result(fileSize, 0);
    in.read(&result[0], fileSize);

    cout << "[" << result << "]" << endl;
  }
  else
  {
    cout << "FILE-NOT-FOUND: " << filePath << endl;
  }

  return 0;
}
