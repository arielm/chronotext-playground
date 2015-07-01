#include "Platform.h"

#include <iostream>

#include "boost/filesystem/fstream.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  auto executablePath = chr::getExecutablePath(argc, argv);

  auto filePath1 = chr::getResourcePath(executablePath, "credits.txt");
  fs::ifstream in1(filePath1, ios::in | ios::binary | ios::ate);
  
  if (in1)
  {
    auto fileSize = in1.tellg();
    in1.seekg(0, ios::beg);

    string result(fileSize, 0);
    in1.read(&result[0], fileSize);

    cout << "[" << result << "]" << endl;
  }
  else
  {
    cout << "FILE-NOT-FOUND: " << filePath1 << endl;
  }

  // ---

  auto filePath2 = chr::getResourcePath(executablePath, "2008.547.1crop_4.jpg");
  fs::ifstream in2(filePath2, ios::in | ios::binary | ios::ate);

  if (in2)
  {
    auto fileSize = in2.tellg();
    in2.seekg(0, ios::beg);

    cout << fileSize << endl;
  }
  else
  {
    cout << "FILE-NOT-FOUND: " << filePath2 << endl;
  }

  return 0;
}
