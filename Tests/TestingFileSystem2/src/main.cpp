#include "Platform.h"

#include <iostream>

#include "boost/filesystem/fstream.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

void* _malloc(size_t size);
void* _realloc(void *ptr, size_t size);
void _free(void *ptr);

#define STBI_MALLOC(sz)    _malloc(sz)
#define STBI_REALLOC(p,sz) _realloc(p,sz)
#define STBI_FREE(p)       _free(p)

#include "stb_image.h"

using namespace std;

void* _malloc(size_t size)
{
  cout << "malloc " << size << endl;
  return malloc(size);
}

void* _realloc(void *ptr, size_t size)
{
  cout << "realloc " << size << endl;
  return realloc(ptr, size);
}

void _free(void *ptr)
{
  cout << "free" << endl;
  free(ptr);
}

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

  string fileName2 = chr::getResourcePath(executablePath, "2008.547.1crop_4.jpg").string();
  int x, y, comp;

  stbi_uc *data = stbi_load(fileName2.data(), &x, &y, &comp, 0);

  if (data)
  {
    cout << x << "x" << y << " (" << comp << ")" << endl;
    stbi_image_free(data);
  }
  else
  {
    cout << "ERROR WITH: " << fileName2 << endl;
  }

#if defined(CHR_PLATFORM_MINGW)
  cout << chr::checkResource(128) << endl;
  cout << chr::checkResource(129) << endl;
#endif

  return 0;
}
