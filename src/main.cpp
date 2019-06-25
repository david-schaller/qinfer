#include <iostream>

#include "qinfer.h"

auto
main(int argc, char* argv[]) -> int
{
  constexpr auto multiplier = 42;
  int i = 0;

  std::cout << "Enter a number" << std::endl;
  std::cin >> i;

  i *= multiplier;

  std::cout << "Magic number: " << i << std::endl;
  std::cout << test << std::endl;

  return 0;
}
