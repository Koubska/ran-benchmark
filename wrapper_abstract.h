#pragma once

#include <initializer_list>

template<typename Poly>
auto discriminant(const Poly& p) {
    return resultant(p, p.derivative());
}

// PolyWrapper operator+(const PolyWrapper& lhs, const PolyWrapper& rhs);
// PolyWrapper operator-(const PolyWrapper& lhs, const PolyWrapper& rhs);
// PolyWrapper operator*(const PolyWrapper& lhs, const PolyWrapper& rhs);

class Wrapper {
public:
    using PolyInit = std::initializer_list<int>;

    // Set variable of given id to RAN given by polynomial and interval
    //void set_variable(VARIABLE var, PolyInit poly, int low, int high) = 0;
};