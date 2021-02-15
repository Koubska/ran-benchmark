#pragma once

#include <gap/libgap-api.h>
#include <gap/objects.h>

#include <cstddef>
#include <iostream>

namespace gap {

void test() {
  auto res = GAP_EvalString("x:=Indeterminate(Rationals,\"x\");");
  PrintObj(res);
}

} // namespace gap