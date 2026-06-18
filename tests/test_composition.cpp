/// Basic test suite for libcomposition using plain assert()

#include "composition.h"
#include <cassert>
#include <cmath>
#include <cstdio>

/// Tolerance for floating point comparisons
static const double TOL = 1e-9;

static bool nearlyEqual(double a, double b, double tol = TOL)
{
    return std::fabs(a - b) < tol;
}

/// Binary Fe-C alloy for testing
#define FOR_FE_C_ELEMENTS(DO)  \
    DO(Fe, false, false, true) \
    DO(C, false, true, false)

MAKE_COMPOSITION_CLASS(CompositionFeC, FOR_FE_C_ELEMENTS)

/// Ternary Fe-C-Mn alloy for testing
#define FOR_STEEL_ELEMENTS(DO) \
    DO(Fe, false, false, true) \
    DO(C, false, true, false)  \
    DO(Mn, false, false, false)

MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_STEEL_ELEMENTS)

/// Test: setting mole fraction X and checking weight fraction W (binary Fe-C)
/// C is interstitial: X_C is relative to substitutional sublattice (X_Fe = 1.0)
/// W_C = (X_C * M_C) / (X_C * M_C + X_Fe * M_Fe)
static void test_SetXCheckWBinary()
{
    CompositionFeC comp;
    comp.C.SetX(0.05);
    comp.UpdateFractions();

    double xC = comp.C.GetX();
    double xFe = comp.Fe.GetX();
    double numerator = xC * PeriodicTable::C.MolarMass;
    double denominator = xC * PeriodicTable::C.MolarMass + xFe * PeriodicTable::Fe.MolarMass;
    double expectedWC = numerator / denominator;

    assert(nearlyEqual(comp.C.GetW(), expectedWC));
    assert(nearlyEqual(comp.C.GetX(), 0.05));
    printf("PASS: test_SetXCheckWBinary\n");
}

/// Test: setting weight fraction W and checking mole fraction X (binary Fe-C)
/// W_C = 0.01
/// X_C = (W_C / M_C) / (W_C / M_C + W_Fe / M_Fe)
static void test_SetWCheckXBinary()
{
    CompositionFeC comp;
    comp.C.SetW(0.01);
    comp.UpdateFractions();

    double nC = 0.01 / PeriodicTable::C.MolarMass;
    double nFe = 0.99 / PeriodicTable::Fe.MolarMass;
    double expectedXC = nC / (nC + nFe);

    assert(nearlyEqual(comp.C.GetX(), expectedXC));
    assert(nearlyEqual(comp.C.GetW(), 0.01));
    printf("PASS: test_SetWCheckXBinary\n");
}

/// Test: site fractions of substitutional elements sum to 1
static void test_USubstitutionalSumToOne()
{
    CompositionSteel comp;
    comp.C.SetX(0.05);
    comp.Mn.SetX(0.02);
    comp.UpdateFractions();

    double sumU = 0.0;
    for (const auto& el : comp.GetElements()) {
        if (!el.IsInterstitial()) {
            sumU += el.GetU();
        }
    }
    assert(nearlyEqual(sumU, 1.0, 1e-9));
    printf("PASS: test_USubstitutionalSumToOne\n");
}

/// Test: mole fractions of all elements sum to 1
static void test_XAllSumToOne()
{
    CompositionSteel comp;
    comp.C.SetX(0.05);
    comp.Mn.SetX(0.02);
    comp.UpdateFractions();

    double sumX = 0.0;
    for (const auto& el : comp.GetElements()) {
        sumX += el.GetX();
    }
    assert(nearlyEqual(sumX, 1.0, 1e-9));
    printf("PASS: test_XAllSumToOne\n");
}

/// Test: weight fractions of all elements sum to 1
static void test_WAllSumToOne()
{
    CompositionSteel comp;
    comp.C.SetX(0.05);
    comp.Mn.SetX(0.02);
    comp.UpdateFractions();

    double sumW = 0.0;
    for (const auto& el : comp.GetElements()) {
        sumW += el.GetW();
    }
    assert(nearlyEqual(sumW, 1.0, 1e-9));
    printf("PASS: test_WAllSumToOne\n");
}

/// Test: U-fraction (site fraction) of interstitial C
/// U_C = X_C / (1 - X_C) where X_C is the mole fraction among substitutional sublattice
static void test_UFraction_Interstitial()
{
    CompositionFeC comp;
    comp.C.SetX(0.05);
    comp.UpdateFractions();

    // U_C = X_C / X_Fe (ratio of interstitial to substitutional mole fraction)
    double expectedUC = comp.C.GetX() / comp.Fe.GetX();
    assert(nearlyEqual(comp.C.GetU(), expectedUC, 1e-9));
    printf("PASS: test_UFraction_Interstitial\n");
}

/// Test: locking composition prevents variable element fractions from changing
static void test_LockComposition()
{
    CompositionSteel comp;
    comp.C.SetX(0.05);
    comp.Mn.SetX(0.02);
    comp.UpdateFractions();
    comp.LockComposition();

    double xCBefore = comp.C.GetX();
    double xMnBefore = comp.Mn.GetX();

    // Attempt to change fractions while locked — should have no effect
    comp.C.SetX(0.10);
    comp.Mn.SetX(0.10);
    comp.UpdateFractions();

    assert(nearlyEqual(comp.C.GetX(), xCBefore));
    assert(nearlyEqual(comp.Mn.GetX(), xMnBefore));
    printf("PASS: test_LockComposition\n");
}

/// Test: unlocking composition allows fractions to change again
static void test_UnlockComposition()
{
    CompositionSteel comp;
    comp.C.SetX(0.05);
    comp.Mn.SetX(0.02);
    comp.UpdateFractions();
    comp.LockComposition();
    comp.UnlockComposition();

    comp.C.SetX(0.10);
    comp.UpdateFractions();

    assert(nearlyEqual(comp.C.GetX(), 0.10));
    printf("PASS: test_UnlockComposition\n");
}

int main()
{
    test_SetXCheckWBinary();
    test_SetWCheckXBinary();
    test_USubstitutionalSumToOne();
    test_XAllSumToOne();
    test_WAllSumToOne();
    test_UFraction_Interstitial();
    test_LockComposition();
    test_UnlockComposition();

    printf("All tests passed.\n");
    return 0;
}
