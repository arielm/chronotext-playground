#include <iostream>
 
#include "boost/filesystem/fstream.hpp"

using namespace std;

namespace fs = boost::filesystem;

int main(int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    cout << "{" << argv[i] << "}" << endl;
  }

  fs::path filePath1 = fs::path("resources") / "credits.txt";
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
}
