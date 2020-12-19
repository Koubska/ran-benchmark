#pragma once

#include <initializer_list>
#include <map>
#include <string>

class Wrapper {
public:
  using PolyInit = std::initializer_list<int>;

  // Set variable of given id to RAN given by polynomial and interval
  // void set_variable(VARIABLE var, PolyInit poly, int low, int high) = 0;
};