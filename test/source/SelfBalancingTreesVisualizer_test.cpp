#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "SelfBalancingTreesVisualizer" ? 0 : 1;
}
