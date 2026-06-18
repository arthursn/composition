# libcomposition

## Background

Converting between atomic and weight composition bases is straightforward when all fractions are given in the same base: knowing the molar masses of the elements, one can compute the average molar mass and convert individual fractions. However, when fractions of some elements are given in the atomic base and others in the weight base, the problem becomes more involved. The conversion equations are derived in [docs > conversionComposition](docs/conversionComposition.md).

## Design

A natural way to represent a composition in C++ is a class with one member variable per element, allowing direct access like `comp.Fe` or `comp.C`. The challenge is making this flexible — different use cases require different sets of elements — without sacrificing performance. A map-based approach (e.g., `std::map<std::string, ElementData>`) would allow dynamic element sets but at the cost of slow key lookups.

The solution here is compile-time code generation using the [X-macro](https://en.wikipedia.org/wiki/X_Macro) pattern — a C preprocessor metaprogramming technique. The user defines the element set in a `FOR_ELEMENTS` macro, and `MAKE_COMPOSITION_CLASS` expands it into a complete, fully typed class at preprocessing time. The result is a class with named member variables per element, with no runtime overhead over a hand-written class.

## Usage

Define your element set and generate the class in a header or at the top of your source file:

```cpp
#include "composition.h"

/* Define the elements. Arguments follow the ElementData constructor:
   element symbol, isVariable, isInterstitial, isMajor */
#define FOR_ELEMENTS(DO)       \
    DO(Fe, false, false, true) \
    DO(C, true, true)          \
    DO(Mn, true)               \
    DO(Si)

MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_ELEMENTS)
```

`MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_ELEMENTS)` expands to:

```cpp
class CompositionSteel : public Composition {
public:
    ElementData Fe = ElementData(PeriodicTable::Fe, false, false, true);
    ElementData C = ElementData(PeriodicTable::C, true, true);
    ElementData Mn = ElementData(PeriodicTable::Mn, true);
    ElementData Si = ElementData(PeriodicTable::Si);

private:
    VectorElementPointers getElementPointers() { return { &Fe, &C, &Mn, &Si }; }
    VectorConstElementPointers getElementPointers() const { return { &Fe, &C, &Mn, &Si }; }

public:
    CompositionSteel() : Composition() { updatePointers(); }
};
```

The arguments to `ElementData` are, in order:
- `element`: element symbol (Title Case), whose properties are fetched from `periodictable.h`
- `isVariable`: whether the element fraction is allowed to change when the composition is locked (see [Locking compositions](#locking-compositions))
- `isInterstitial`: whether the element is interstitial (`true`) or substitutional (`false`)
- `isMajor`: whether the element is the major element (e.g., Fe in steel)

### Setting and reading fractions

```cpp
CompositionSteel comp;
comp.C.SetW(.5e-2);
comp.Mn.SetW(2e-2);
comp.Si.SetX(1e-3);
comp.UpdateFractions();
comp.Print();
```

```
        | At. fraction (X) | Wt. fraction (W) | Site fraction (U)
  ------+------------------+------------------+-------------------
    Fe* |         0.956238 |         0.974487 |          0.978561
     C  |        0.0228126 |            0.005 |         0.0233452
    Mn  |        0.0199494 |             0.02 |         0.0204152
    Si  |            0.001 |      0.000512517 |        0.00102335
```

Individual fractions can be read directly:

```cpp
comp.C.GetX();   // 0.0228126
comp.Si.GetW();  // 0.000512517
```

Elements can also be accessed by symbol string, though this is slower:

```cpp
comp["C"].GetX();
comp["Si"].GetW();
```

And iterated over:

```cpp
for (ElementData& el : comp.GetElements()) {
    std::cout << el.GetSymbol() << ": " << el.GetW() << std::endl;
}
```

## Locking compositions

When modelling local composition changes in a material, changing the fraction of one element affects all others through the average molar mass. Often the intent is to change one element's fraction while keeping the **site fractions** of all others fixed. `LockComposition()` achieves this by fixing the site fractions of all elements marked as non-variable.

```cpp
comp.LockComposition();
comp.C.SetX(3e-2);
comp.Si.SetX(3e-2); // Error: Si is locked
comp.Print();
```

```
Cannot set locked X(Si) composition
        | At. fraction (X) | Wt. fraction (W) | Site fraction (U)
  ------+------------------+------------------+-------------------
    Fe* |         0.949205 |         0.972907 |          0.978561
     C  |             0.03 |       0.00661328 |         0.0309278
    Mn  |        0.0198027 |        0.0199676 |         0.0204152
   >Si< |      0.000992645 |      0.000511686 |        0.00102335
```

The site fractions of all elements except C remain unchanged.

## Compilation

CMake is used to build the source files as a shared library:

```sh
cmake -B build
cmake --build build
```

Link the resulting library with your project and add the `include` directory to your include path. No external dependencies are required.
