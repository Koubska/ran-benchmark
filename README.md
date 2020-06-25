# Available libraries

Collected from:
- https://en.wikipedia.org/wiki/List_of_computer_algebra_systems

## Candidates
### libpoly
### carl



## Not considered

- Not open-source / free: LiveMath, Macsyma, Magma, Maple, Mathcad, Mathematica, MATLAB, SMath Studio, TI-Nspire CAS, Wolfram Apha
- Discontinued, no active development: Axiom, Derive, Erable / ALGB, KANT / KASH, Magnus, Mathomatic, MuMATH, MuPAD, OpenAxiom
- Not usable from C++: SageMath, SymPy
- Does not provide desired functionality:
  - Cadabra (very specific to theoretical physics)
  - CoCoALib (no real algebraic numbers)
  - Fermat (no real algebraic numbers, one-man show)
  - FORM (also for physics)
  - GiNaC (no real root isolation)
  - Maxima (no real root isolation)
  - Octave (no arbitrary-precision numbers)
  - PARI/GP (no real algebraic numbers)
  - Scilab (meant as Matlab alternative)
  - Xcas / Giac (no real root isolation, based on PARI and CoCoA, French)
  - Yacas (no real algebraic numbers)

## To be considered
### FriCAS
Fork of Axiom. Not yet clear, whether it can be used from C++.

### GAP
### Macaulay2
Uses PARI for some backend computations anyway. Can it do real root isolation?

### Reduce
- real root isolation (ISOLATER from ROOTS package)
- algebraic numbers (from ARNUM package), though unclear how well this is integrated
- How to use as library?

### SINGULAR
Provides a library (libsingular4-dev).
What about real root isolation?
