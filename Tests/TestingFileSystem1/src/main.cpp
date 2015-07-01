#include "Platform.h"

#include <iostream>

#include "boost/filesystem/fstream.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include "stb_image.h"

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
  int x, y, comp;

  if (stbi_info(filePath2.string().data(), &x, &y, &comp))
  {
    cout << x << "x" << y << " (" << comp << ")" << endl;
  }
  else
  {
    cout << "ERROR WITH: " << filePath2 << endl;
  }

  return 0;
}
