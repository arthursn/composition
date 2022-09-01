# `libcomposition`

# Background

The conversion between atomic and weight composition bases is normally a very straightforward problem. Having the molar masses of the elements, one can easily determine the average molar mass and from then convert the individual fractions of each element.

However, if for some reason the composition is provided by defined the fractions of some elements in the atomic base and others in the weight base, then the problem becomes a little bit more complicated. Yet, it is perfectly possible to make the conversion. The equations for base conversion are presented in the supplementary document in [docs > conversionComposition](docs/conversionComposition.md).

Okay, having the equations remove one thing out of the way. Now we want to write a C++ that does that. But we want the class to be dynamic, supporting multiple elements, but at the same time fast. A very bad idea is to use use `std::map` for that, because accessing the elements of the map by key is quite slow. It would be nicer if we could access the fractions of each element by symbol, but having the symbols as public member variables of the composition class.

# Usage

The solution I found is MACROS! In `composition.h` we define an abstract class `Composition`. `Composition` itself has no elements defined. So, by itself it is useless. In your project, you have to create a class derived from `Composition` in your file preamble or in a separate header file. This can be done manually, or by using the `MAKE_COMPOSITION_CLASS` macro:

```cpp
#include "composition.h"

/* The defined steel elements. The DO notation is used here due to the X Macro
technique used for making this class dynamic (see https://en.wikipedia.org/wiki/X_Macro)
The arguments in the DO macro call follow the Constructor of ElementData,
i.e., are respectively the element symbol, isVariable, isInterstitial, and
isMajor */
#define FOR_ELEMENTS(DO)       \
    DO(Fe, false, false, true) \
    DO(C, true, true)          \
    DO(Mn, true)               \
    DO(Si)

// Makes the CompositionSteel class
MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_ELEMENTS)
```

Notice that you also have to defined a `FOR_ELEMENTS` macro with this funny `DO` syntax. This is because of the X Macro technique that we use to make the class dynamic (see https://en.wikipedia.org/wiki/X_Macro).

In the example above `MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_ELEMENTS)` will be expanded in the pre-processing as the following `CompositionSteel` class:

```cpp
class CompositionSteel : public Composition {
public:
    /* Define elements (ElementData) as public members */
    ElementData Fe = ElementData(PeriodicTable::Fe, false, false, true);
    ElementData C = ElementData(PeriodicTable::C, true, true);
    ElementData Mn = ElementData(PeriodicTable::Mn, true);
    ElementData Si = ElementData(PeriodicTable::Si);

private:
    /* Override virtual functions */
    VectorElementPointers getElementPointers() { return { &Fe, &C, &Mn, &Si }; }
    VectorConstElementPointers getElementPointers() const { return { &Fe, &C, &Mn, &Si }; }
    
public:
    /* Constructor */
    CompositionSteel()
        : Composition()
    {
        updatePointers();
    }
};
```

The `ElementData` class stores the fractions of the element. When defining the `FOR_ELEMENTS` macro or instantiating `ElementData`, the arguments represent the element symbol (whose properties are fetched from `periodictable.h`), `isInterstitial` (i.e., where is a interstitial element), `isVariable` (whether the element is allowed to vary when the composition is locked, see [Locking compositions](#Locking-compositions)), and `isMajor` (whether the element is the major element).

Then, in the body of your code, you can use the newly created class at will:

```cpp
CompositionSteel comp;
comp.C.SetW(.5e-2);
comp.Mn.SetW(2e-2);
comp.Si.SetX(1e-3);
comp.UpdateFractions();
comp.Print();
```

... will print:

```
        | At. fraction (X) | Wt. fraction (W) | Site fraction (U)
  ------+------------------+------------------+-------------------
    Fe* |         0.956238 |         0.974487 |          0.978561
     C  |        0.0228126 |            0.005 |         0.0233452
    Mn  |        0.0199494 |             0.02 |         0.0204152
    Si  |            0.001 |      0.000512517 |        0.00102335
```

For getting the fraction of an individual element:

```cpp
comp.C.GetX(); // returns 0.0228126 (double)
comp.Si.GetW(); // returns 0.000512517 (double)
```

Even though it is slow, you can also access the element using the map syntax:

```cpp
comp["C"].GetX();
comp["Si"].GetW();
```

You can loop through all elements with an iterator:

```cpp
// Will print the weight fraction of all elements
for (ElementData& el : comp.GetElements()) {
    std::cout << el.GetSymbol() << ": " << el.GetW() << std::endl;
}

```
## Locking compositions

When we are modelling local changes of composition in a material, one tricky thing that we face is that changing the fraction of one element should change the fractions of all other elements, as the average molar mass changes! Normally, the intent is that you want to change the fraction of an element and keep the **site fractions** of all other elements fixed. So in the Composition class, there is a method `LockComposition` that locks the site fractions of all elements that **are not allowed to vary** in place. In the example above, C and Mn were defined as elements allowed to vary. When the composition is locked, then we obtain:

```cpp
comp.LockComposition();
comp.C.SetX(3e-2);
comp.Si.SetX(3e-2); // This will result in a ERROR message because Si is locked
comp.Print(); // The Print function calls UpdateFractions() automatically
```

```
        | At. fraction (X) | Wt. fraction (W) | Site fraction (U)
  ------+------------------+------------------+-------------------
    Fe* |         0.949205 |         0.972907 |          0.978561
     C  |             0.03 |       0.00661328 |         0.0309278
    Mn  |        0.0198027 |        0.0199676 |         0.0204152
   >Si< |      0.000992645 |      0.000511686 |        0.00102335
```

Notice that the site fractions of all elements (except C) remained the same.

# Compilation

CMake is used for building the source files as a shared library:

```sh
mkdir build
cd build
cmake ..
make
```

Then you can link the resulting library file with your project. Be sure to set correctly the path to  the `include` directory to be able to import the `composition.h` header. Of course you can use your own building system to compile the code. The class does not require any dependency.
