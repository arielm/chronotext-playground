/*
 * TODO:
 * 
 * - ANDROID: COPY resources FOLDER TO DEVICE
 */

#include "Platform.h"

#include <iostream>

#include "boost/filesystem/fstream.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // for (int i = 0; i < argc; i++) { cout << "\t" << argv[i] << endl; }

  cout << "PLATFORM: " << CHR_PLATFORM << endl;

  auto exePath = chr::getExecutablePath(argc, argv);
  cout << "EXE PATH: " << exePath.string() << endl;

  auto filePath = chr::getResourcePath(exePath, "test.txt");
  cout << "EXISTS: " << fs::exists(filePath) << endl;

  fs::ifstream in(filePath, ios::in | ios::binary | ios::ate);
  
  if (in)
  {
    auto fileSize = in.tellg();
    in.seekg(0, ios::beg);

    string result(fileSize, 0);
    in.read(&result[0], fileSize);

    cout << "[" << result << "]" << endl;
  }

  return 0;
}
