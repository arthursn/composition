#include "composition.h"

/// The defined steel elements. The DO notation is used here due to the X Macro
/// technique used for making this class dynamic (see https://en.wikipedia.org/wiki/X_Macro)
/// The arguments in the DO macro call follow the Constructor of ElementData,
/// i.e., are respectively the element symbol, isInterstitial, isVariable, and
/// isMajor
#define FOR_STEEL_ELEMENTS(DO) \
    DO(Fe, false, false, true) \
    DO(C, true, true)          \
    DO(N, false, true)         \
    DO(Mn, true)               \
    DO(Al)                     \
    DO(Si)                     \
    DO(P)                      \
    DO(S)                      \
    DO(Ti)                     \
    DO(Cr)                     \
    DO(Ni)                     \
    DO(Nb)                     \
    DO(Mo)

MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_STEEL_ELEMENTS)

int main()
{
    CompositionSteel comp;
    comp.C.SetW(.5e-2);
    comp.Mn.SetW(2e-2);
    comp.Cr.SetW(3e-2);
    comp.LockComposition();

    comp.Print();

    return 0;
}