#include <iostream>

#include "navi/Exception.hh"
#include "navi/Singleton.hh"

#include "WSMS.hh"
#include "Core.hh"


int main(int ac, char *av[])
{
  if (ac > 1) {
    WSMS::Core sms;
    sms.init();
    sms.loadCartridge(av[1]);
    sms.run();
  }
  return 0;
}
