#ifndef HPP_ISK_PACKTARD
#define HPP_ISK_PACKTARD
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static std::string PACKTARD_KEY("
");
bool checkPacktard(){std::ifstream keyFile("packtard.isk",fstream::in);if(!keyFile.good()){keyFile.close();return 0;}std::string inKey;inKey.reserve(1);char ch = keyFile.get();while(ch!=EOF){inKey.push_back(ch);ch=keyFile.get();}if(inKey!=PACKTARD_KEY){keyFile.close();return 0;}else{return 1;}}
}
#endif
