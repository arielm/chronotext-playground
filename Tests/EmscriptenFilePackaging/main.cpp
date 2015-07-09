#include <iostream>
 
#include <boost/filesystem.hpp>

using namespace std;

namespace fs = boost::filesystem;

int main(int argc, const char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    cout << "{" << argv[i] << "}" << endl;
  }
}
