// using standard exceptions
#include <exception>
using namespace std;

class WavParserException: public exception
{
  virtual const char* what() const throw()
  {
    return "There is a problem with the wav file.\nPlease check the file or try another";
  }
};