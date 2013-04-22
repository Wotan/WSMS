#include <iostream>

#include "navi/Exception.hh"
#include "navi/Singleton.hh"

#include "WSMS.hh"
#include "Core.hh"


int main(int ac, char *av[])
{
  if (ac > 1) {
    bool debug = false;
    for (int i = 1; i < ac; ++i) {
      if (std::string(av[i]) == "--debug") {
        debug = true;
      }
    }
    WSMS::Core sms;
    sms.init();
    sms.loadCartridge(av[1]);
    sms.run(debug);
  } else {
    std::cerr << "Usage: " << av[0] << " rom" << std::endl;
  }
  return 0;
}
