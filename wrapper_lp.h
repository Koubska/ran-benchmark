#pragma once

/*
 * Wrapper for libpoly. Download and build libpoly somewhere and adapt the path here.
 */

#include "../libpoly/include/assignment.h"
#include "../libpoly/include/algebraic_number.h"
#include "../libpoly/include/dyadic_interval.h"
#include "../libpoly/include/integer.h"
#include "../libpoly/include/polynomial.h"
#include "../libpoly/include/polynomial_context.h"
#include "../libpoly/include/upolynomial.h"
#include "../libpoly/include/value.h"
#include "../libpoly/include/variable_db.h"
#include "../libpoly/include/variable_order.h"

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "wrapper_abstract.h"


std::ostream& operator<<(std::ostream& os, const lp_algebraic_number_t& ran) {
    return os << lp_algebraic_number_to_string(&ran);
}
std::ostream& operator<<(std::ostream& os, const lp_value_t& v) {
    return os << lp_value_to_string(&v);
}
std::ostream& operator<<(std::ostream& os, const lp_assignment_t& a) {
    return os << lp_assignment_to_string(&a);
}

namespace lp {

    template<typename T>
    using deleting_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

    void value_deleter(lp_value_t* ptr) {
        lp_value_delete(ptr);
    }

    class Value {
        friend std::ostream& operator<<(std::ostream& os, const Value& v);
        deleting_unique_ptr<lp_value_t> mValue;
    public:
        Value(): mValue(lp_value_new(LP_VALUE_NONE, nullptr), value_deleter) {}
        Value(lp_value_t* ptr): mValue(ptr, value_deleter) {}
        Value(const Value& val): Value(lp_value_new_copy(val.get())) {}

        lp_value_t* get() {
            return mValue.get();
        }
        const lp_value_t* get() const {
            return mValue.get();
        }
    };
    std::ostream& operator<<(std::ostream& os, const Value& v) {
        return os << *v.get();
    }

    void polynomial_deleter(lp_polynomial_t* ptr) {
        lp_polynomial_delete(ptr);
    }

    class Poly {
        friend std::ostream& operator<<(std::ostream& os, const Poly& p);
    private:
        deleting_unique_ptr<lp_polynomial_t> mPoly;
    public:
        Poly(): mPoly(lp_polynomial_new(ctx.get()), polynomial_deleter) {}
        Poly(lp_polynomial_t* poly): mPoly(poly, polynomial_deleter) {}
        Poly(int i, lp_variable_t x, unsigned n): mPoly(lp_polynomial_alloc(), polynomial_deleter) {
            lp_integer_t it;
            lp_integer_construct_from_int(lp_Z, &it, i);
            lp_polynomial_construct_simple(get(), ctx.get(), &it, x, n);
            lp_integer_destruct(&it);
        }

        lp_polynomial_t* get() {
            return mPoly.get();
        }
        const lp_polynomial_t* get() const {
            return mPoly.get();
        }

        auto derivative() const {
            Poly res;
            lp_polynomial_derivative(res.get(), get());
            return res;
        }

        std::size_t degree() const {
            return lp_polynomial_degree(get());
        }

        static deleting_unique_ptr<lp_variable_db_t> db;
        static deleting_unique_ptr<lp_variable_order_t> order;
        static deleting_unique_ptr<lp_polynomial_context_t> ctx;
    };
    
    deleting_unique_ptr<lp_variable_db_t> Poly::db(lp_variable_db_new(), [](lp_variable_db_t* ptr){ lp_variable_db_detach(ptr); });
    deleting_unique_ptr<lp_variable_order_t> Poly::order(lp_variable_order_new(), [](lp_variable_order_t* ptr){ lp_variable_order_detach(ptr); });
    deleting_unique_ptr<lp_polynomial_context_t> Poly::ctx(lp_polynomial_context_new(lp_Z, Poly::db.get(), Poly::order.get()), [](lp_polynomial_context_t* ptr){ lp_polynomial_context_detach(ptr); });

    std::ostream& operator<<(std::ostream& os, const Poly& p) {
        return os << lp_polynomial_to_string(p.get());
    }

    Poly operator+(const Poly& lhs, const Poly& rhs) {
        Poly res;
        lp_polynomial_add(res.get(), lhs.get(), rhs.get());
        return res;
    }
    Poly operator-(const Poly& lhs, const Poly& rhs) {
        Poly res;
        lp_polynomial_sub(res.get(), lhs.get(), rhs.get());
        return res;
    }
    Poly operator*(const Poly& lhs, const Poly& rhs) {
        Poly res;
        lp_polynomial_mul(res.get(), lhs.get(), rhs.get());
        return res;
    }

    Poly resultant(const Poly& p, const Poly& q) {
        Poly res;
        lp_polynomial_resultant(res.get(), p.get(), q.get());
        return res;
    }

    auto value_from(const lp_algebraic_number_t& ran) {
        return lp_value_new(lp_value_type_t::LP_VALUE_ALGEBRAIC, &ran);
    }

    auto get_assignment(std::initializer_list<lp_variable_t> vars, std::initializer_list<lp_value_t*> values) {
        auto a = lp_assignment_new(Poly::db.get());
        assert(vars.size() == values.size());
        auto v1 = vars.begin();
        auto v2 = values.begin(); 
        for (; v1 != vars.end(); ++v1, ++v2) {
            lp_assignment_set_value(a, *v1, *v2);
        }
        return *a;
    }

    std::vector<lp_value_t> isolate_real_roots(const Poly& p, const lp_assignment_t& assignment) {
        lp_value_t* roots = new lp_value_t[p.degree()];
        std::size_t roots_size;
        lp_polynomial_roots_isolate(p.get(), &assignment, roots, &roots_size);
        std::vector<lp_value_t> res;
        for (std::size_t i = 0; i < roots_size; ++i) {
            res.emplace_back(roots[i]);
        }
        return res;
    }

    class LibPolyWrapper: public Wrapper {
        auto get_upoly(const PolyInit& poly) {
            int coeffs[poly.size()];
            std::size_t cur = 0;
            for (int c: poly) coeffs[cur++] = c;
            return lp_upolynomial_construct_from_int(lp_Z, poly.size()-1, coeffs);
        }
        auto get_interval(int l, int u) {
            lp_dyadic_interval_t i;
            lp_dyadic_interval_construct_from_int(&i, l, 1, u, 1);
            return i;
        }
        
        lp_assignment_t* assignment = lp_assignment_new(Poly::db.get());

    public:

        ~LibPolyWrapper() {
            lp_assignment_delete(assignment);
        }
        
        using Poly = lp::Poly;

        lp_variable_t fresh_variable(const char* name) {
            return lp_variable_db_new_variable(Poly::db.get(), name);
        }

        void set_variable(lp_variable_t var, PolyInit poly, int low, int high) {
            auto p = get_upoly(poly);
            auto i = get_interval(low, high);
            lp_algebraic_number_t ran;
            lp_algebraic_number_construct(&ran, p, &i);
            auto val = lp_value_new(lp_value_type_t::LP_VALUE_ALGEBRAIC, &ran);
            lp_assignment_set_value(assignment, var, val);
            lp_value_delete(val);
            lp_dyadic_interval_destruct(&i);
            lp_algebraic_number_destruct(&ran);
        }

        std::vector<Value> isolate_real_roots(const Poly& p) {
            lp_value_t* roots = new lp_value_t[p.degree()];
            std::size_t roots_size;
            lp_polynomial_roots_isolate(p.get(), assignment, roots, &roots_size);
            std::vector<Value> res;
            res.emplace_back(lp_value_new(LP_VALUE_MINUS_INFINITY, nullptr));
            for (std::size_t i = 0; i < roots_size; ++i) {
                res.emplace_back(lp_value_new_copy(&roots[i]));
            }
            res.emplace_back(lp_value_new(LP_VALUE_PLUS_INFINITY, nullptr));
            for (std::size_t i = 0; i < roots_size; ++i) {
                lp_value_destruct(&roots[i]);
            }
            delete[] roots;
            return res;
        }

        std::vector<Value> lift(const Poly& p, lp_variable_t v) {
            std::vector<Value> res;
            auto roots = isolate_real_roots(p);
            for (std::size_t i = 0; i < roots.size()-1; ++i) {
                res.emplace_back();
                lp_value_get_value_between(roots[i].get(), 1, roots[i+1].get(), 1, res.back().get());
                if (i < roots.size()-2) {
                    res.emplace_back(roots[i+1]);
                }
            }
            return res;
        }
    };


}
