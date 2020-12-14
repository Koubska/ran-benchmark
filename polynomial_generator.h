#pragma once

#include "wrapper_carl.h"
#include "wrapper_lp.h"

class PolynomialGenerator{
    public : 
    static PolynomialGenerator& instance() {
        static PolynomialGenerator generator ; 
        return generator ; 
    }

    carl::Poly& getCarlPoly1(){
        return poly_carl1 ; 
    }

    carl::Poly& getCarlPoly2(){
        return poly_carl2 ; 
    }

    lp::Poly& getLPPoly1(){
        return poly_lp1 ;
    }

    lp::Poly& getLPPoly2(){
        return poly_lp2 ;
    }

    private : 
    carl::Poly poly_carl1 ;
    carl::Poly poly_carl2 ;

    lp::Poly poly_lp1 ; 
    lp::Poly poly_lp2 ; 

    template <typename T> 
    auto createPoly1(T& t){ // -1*z + x + y^2
        using P = typename T::Poly ;
        //Generate Variables
        auto x = t.fresh_variable("x");
        auto y = t.fresh_variable("y");
        auto z = t.fresh_variable("z");

        // Generate Poly
        return (P(1, y, 2) + P(1, x, 1)) - P(1, z, 1);
    }

    template <typename T> 
    auto createPoly2(T& t){ // 3x^2
        using P = typename T::Poly ;
        //Generate Variables
        auto x = t.fresh_variable("x");

        // Generate Poly
        return P(3, x, 2);
    }

    PolynomialGenerator(){
        // Create Wrapper 
        carl::CarlWrapper carl_wrapper;
        lp::LibPolyWrapper lp_wrapper;
        // Create Polynomials
        poly_carl1 = createPoly1(carl_wrapper) ; 
        poly_carl2 = createPoly2(carl_wrapper) ; 
        poly_lp1 = createPoly1(lp_wrapper) ; 
        poly_lp2 = createPoly2(lp_wrapper) ; 

        std::cout << "Called PolyGeneratorConstructor" << std::endl ; 
        std::cout << "Carl: "  << poly_carl1 << "\t" << poly_carl2 << std::endl ;   
        std::cout << "LibPoly: " << poly_lp1 << "\t" << poly_lp2   << std::endl ;   
    }

    PolynomialGenerator(const PolynomialGenerator&) ; 

    PolynomialGenerator & operator = (const PolynomialGenerator &) ;

};