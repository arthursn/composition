#include "composition.h"
#include <cstdio>

// Converts string to title case
static std::string toTitleCase(const std::string& str)
{
    std::string strNew = str;
    for (char& c : strNew)
        c = tolower(c);
    strNew[0] = toupper(strNew[0]);
    return strNew;
}

/** @brief Constructor of ElementData
 * 
 * @param element The element from the periodic table (see the PeriodicTable namespace)
 * @param isInterstitial Boolean. If true, it is interstitial element (substitutional, otherwise) (Default: false)
 * @param isVariable Boolean. If true, composition of element is allowed to change even if Composition is locked (Default: false)
 * @param isMajor Boolean. If true, it is major element (Default: false)
 */
ElementData::ElementData(const PeriodicTable::Element& element, bool isInterstitial, bool isVariable, bool isMajor)
    : ElementData()
{
    mvSymbol = element.Symbol;
    mvMolarMass = element.MolarMass;
    mvIsInterstitial = isInterstitial;
    mvIsVariable = isVariable;
    mvIsMajor = isMajor;
}

/** @brief Set mole fraction of element
 * 
 * @param x Mole fraction
 */
void ElementData::SetX(double x)
{
    if (mvIsAllowedToVary) {
        mvUserX = mvX = x;
        mvUserW = mvW = mvU = 0.0;
        mvIsUpdated = false;
    } else {
        LOG_F(ERROR, "Cannot set locked X(%s) composition", mvSymbol.c_str());
    }
}

/** @brief Set weight fraction of element
 * 
 * @param w Weight fraction
 */
void ElementData::SetW(double w)
{
    if (mvIsAllowedToVary) {
        if (!mvIsCompositionLocked) {
            mvUserW = mvW = w;
            mvUserX = mvX = mvU = 0.0;
            mvIsUpdated = false;
        } else {
            LOG_F(ERROR, "Setting mass fraction W(%s) not supported when composition is locked. Try setting in atomic fraction (ElementData::SetX) instead", mvSymbol.c_str());
        }
    } else {
        LOG_F(ERROR, "Cannot set locked W(%s) composition", mvSymbol.c_str());
    }
}

/// @brief Updates the pointers to the elements (mvpMajorElement, mvAlloyingElements, etc...)
/// This function needs to be called only once. If it is called a second time, it does nothing
void CompositionBase::updatePointers()
{
    // Do not update composition pointers if they are already updated
    if (mvArePointersUpdated) {
        return;
    }

    mvVariableInterstitialElements.clear();
    mvFixedInterstitialElements.clear();
    mvVariableSubstitutionalElements.clear();
    mvFixedSubstitutionalElements.clear();

    size_t cntMajor = 0;
    for (ElementPointer pEl : getElementPointers()) {
        if (pEl->mvIsMajor) {
            if (cntMajor > 1) {
                LOG_F(ERROR, "CompositionBase::updatePointers: Error! More than one major elements defined (%s and %s)", (mvpMajorElement->mvSymbol).c_str(), (pEl->mvSymbol).c_str());
                return;
            }
            mvpMajorElement = pEl;
            cntMajor++;
        } else {
            if (pEl->mvIsInterstitial) {
                if (pEl->mvIsVariable) {
                    mvVariableInterstitialElements.push_back(pEl);
                } else {
                    mvFixedInterstitialElements.push_back(pEl);
                }
            } else {
                if (pEl->mvIsVariable) {
                    mvVariableSubstitutionalElements.push_back(pEl);
                } else {
                    mvFixedSubstitutionalElements.push_back(pEl);
                }
            }
        }
    }

    if (cntMajor == 0) {
        LOG_F(ERROR, "CompositionBase::updatePointers: Error! No major element defined!");
        return;
    }

    mvInterstitialElements = mvVariableInterstitialElements;
    mvInterstitialElements.insert(mvInterstitialElements.end(), mvFixedInterstitialElements.begin(), mvFixedInterstitialElements.end());

    mvSubstitutionalElements = mvVariableSubstitutionalElements;
    mvSubstitutionalElements.insert(mvSubstitutionalElements.end(), mvFixedSubstitutionalElements.begin(), mvFixedSubstitutionalElements.end());

    mvVariableElements = mvVariableInterstitialElements;
    mvVariableElements.insert(mvVariableElements.end(), mvVariableSubstitutionalElements.begin(), mvVariableSubstitutionalElements.end());

    mvFixedElements = mvFixedInterstitialElements;
    mvFixedElements.insert(mvFixedElements.end(), mvFixedSubstitutionalElements.begin(), mvFixedSubstitutionalElements.end());

    mvAlloyingElements = mvInterstitialElements;
    mvAlloyingElements.insert(mvAlloyingElements.end(), mvSubstitutionalElements.begin(), mvSubstitutionalElements.end());

    mvArePointersUpdated = true;
}

/** @brief operator[] for accessing elements by their names 
 * 
 * @param elementSymbol The element name
 * 
 * @return Pointer to ElementData
 */
ElementData& CompositionBase::operator[](const std::string& elementSymbol)
{
    updatePointers();

    std::string elementSymbolTitle = toTitleCase(elementSymbol);
    for (ElementPointer pEl : getElementPointers()) {
        if (pEl->mvSymbol == elementSymbolTitle) {
            return *pEl;
        }
    }

    // Abort (throw exception) if cannot not find the element
    ABORT_F("Element %s is not defined", elementSymbolTitle.c_str());
}

/** @brief const version of operator[] for accessing elements by their names 
 * 
 * @param elementSymbol The element name
 * 
 * @return Const pointer to respective ElementData
 */
const ElementData& CompositionBase::operator[](const std::string& elementSymbol) const
{
    std::string elementSymbolTitle = toTitleCase(elementSymbol);
    for (ConstElementPointer pEl : getElementPointers()) {
        if (pEl->mvSymbol == elementSymbolTitle) {
            return *pEl;
        }
    }

    // Abort (throw exception) if cannot not find the element
    ABORT_F("Element %s is not defined", elementSymbolTitle.c_str());
}

/// @brief Locks composition, i.e., keeps site fraction of non-variable elements fixed
void Composition::LockComposition()
{
    updatePointers();

    updateFractions();

    for (ElementPointer pEl : mvFixedElements) {
        pEl->mvIsAllowedToVary = false;
    }

    for (ElementPointer pEl : mvAlloyingElements) {
        pEl->mvIsCompositionLocked = true;
    }

    mvIsCompositionLocked = true;
}

/// @brief Unlocks composition (see LockComposition)
void Composition::UnlockComposition()
{
    updatePointers();

    for (ElementPointer pEl : mvAlloyingElements) {
        pEl->mvIsAllowedToVary = true;
        pEl->mvIsCompositionLocked = false;
    }

    mvIsCompositionLocked = false;
}

/// @brief Updates fractions
void Composition::UpdateFractions()
{
    updatePointers();

    if (mvpMajorElement == nullptr) {
        LOG_F(ERROR, "Composition::UpdateFractions: Error! No major element defined!");
        return;
    }

    if (!mvIsCompositionLocked) {
        updateFractions();
    } else {
        updateFractionsUFixed();
    }
}

/** @brief Updates the fractions and prints composition
 * 
 * @param verbosity Loguru verbosity level (e.g., 0 [INFO])
 */
void Composition::Print(int verbosity, const char* preamble)
{
    updatePointers();

    if (mvpMajorElement == nullptr) {
        LOG_F(ERROR, "Composition::Print: Error! No major element defined!");
        return;
    }

    UpdateFractions();

    const auto* constThis = this;
    constThis->Print(verbosity, preamble);
}

/** @brief Prints the composition
 * 
 * @param verbosity Loguru verbosity level (e.g., 0 [INFO])
 */
void Composition::Print(int verbosity, const char* preamble) const
{
    char buff[1024]; // This buffer size should be big enough
    std::string log;

    log += std::string(preamble);
    log += "        | At. fraction (X) | Wt. fraction (W) | Site fraction (U)\n";
    log += "  ------+------------------+------------------+-------------------\n";

    for (ConstElementPointer pEl : getElementPointers()) {
        if (pEl->mvX <= 0)
            continue;

        unsigned char pos = pEl->mvIsMajor ? 2 : (pEl->mvIsAllowedToVary ? 1 : 0);
        sprintf(buff, "   %c%2s%c | %16.6g | %16.6g | %17.6g\n",
            ">  "[pos], pEl -> mvSymbol.c_str(), "< *"[pos], pEl -> mvX, pEl -> mvW, pEl -> mvU);
        log += std::string(buff);
    }
    sprintf(buff, "  Average molar mass: %8g\n", mvMolarMassAvg);
    log += std::string(buff);

    VLOG_F((loguru::Verbosity)verbosity, "%s", log.c_str());
}

/** @brief Private implementation of update fractions that is used when 
 * the composition is unlocked
 */
void Composition::updateFractions()
{
    double MMajor = mvpMajorElement->mvMolarMass;
    // MAvgNum: Average molar mass numerator
    // MAvgDen: Average molar mass denominator
    double MAvgNum = MMajor, MAvgDen = 1.0;

    // xSum: sum of the atomic fractions of all atomic elements (excluding major)
    // wSum: sum of the weight fractions of all atomic elements (excluding major)
    double xSum = 0.0, wSum = 0.0;
    // Calculates average molar mass and mole fraction of major element
    for (ElementPointer pEl : mvAlloyingElements) {
        xSum += pEl->mvUserX;
        MAvgNum -= (MMajor - pEl->mvMolarMass) * pEl->mvUserX;

        wSum += pEl->mvUserW / pEl->mvMolarMass;
        MAvgDen += (MMajor / pEl->mvMolarMass - 1.0) * pEl->mvUserW;
    }

    mvMolarMassAvg = MAvgNum / MAvgDen;
    double xMajor = 1.0 - xSum - wSum * mvMolarMassAvg;

    mvpMajorElement->mvX = xMajor;
    mvpMajorElement->mvW = xMajor * MMajor / mvMolarMassAvg;

    // Calculates mole and mass fractions of remaining elements
    for (ElementPointer pEl : mvAlloyingElements) {
        double conversionFactor = mvMolarMassAvg / pEl->mvMolarMass;
        if (pEl->mvUserX > 0)
            pEl->mvW = pEl->mvUserX / conversionFactor;
        else if (pEl->mvUserW > 0)
            pEl->mvX = pEl->mvUserW * conversionFactor;
    }

    double xSumSubstitutional = 1.0;
    // Calculates fraction of substitutional elements
    for (ElementPointer pEl : mvInterstitialElements) {
        xSumSubstitutional -= pEl->mvX;
    }

    // Calculates site fractions U
    mvpMajorElement->mvU = mvpMajorElement->mvX / xSumSubstitutional;
    mvpMajorElement->mvIsUpdated = true;

    for (ElementPointer pEl : mvVariableElements) {
        pEl->mvU = pEl->mvX / xSumSubstitutional;
        pEl->mvIsUpdated = true;
    }

    mvMolarMassAvgFixedPartial = 0.0;
    for (ElementPointer pEl : mvFixedElements) {
        pEl->mvU = pEl->mvX / xSumSubstitutional;
        mvMolarMassAvgFixedPartial += pEl->mvU * (mvpMajorElement->mvMolarMass - pEl->mvMolarMass);
        pEl->mvIsUpdated = true;
    }

    mvXSumSubstitutionalFixedPartial = 1.0;
    for (ElementPointer pEl : mvFixedInterstitialElements) {
        mvXSumSubstitutionalFixedPartial -= pEl->mvX;
    }
}

/** @brief Implementation of update fraction that is used when the composition
 * is locked. It assumes that the u-fractions (site fractions) of the "fixed"
 * or "non-variable" elements does not change
 */
void Composition::updateFractionsUFixed()
{
    double xMSumProduct = 0.0;
    double xSumSubstitutional = mvXSumSubstitutionalFixedPartial;
    double xSumAlloying = 0.0;
    int notUpdatedCounterInterstitial = 0;
    int notUpdatedCounterSubstitutional = 0;

    // Loop through variable interstitial elements to compute the the partial sum xMSumProduct
    // used for determining average molar mass
    for (ElementPointer pEl : mvVariableInterstitialElements) {
        if (!pEl->mvIsUpdated) {
            notUpdatedCounterInterstitial++;
        }
        xSumSubstitutional -= pEl->mvX;
        xMSumProduct += pEl->mvX * (mvpMajorElement->mvMolarMass - pEl->mvMolarMass);
    }

    // Loop through variable interstitial elements to compute the partial sum xMSumProduct
    // used for determining average molar mass
    for (ElementPointer pEl : mvVariableSubstitutionalElements) {
        if (!pEl->mvIsUpdated) {
            xMSumProduct += pEl->mvX * (mvpMajorElement->mvMolarMass - pEl->mvMolarMass);
            notUpdatedCounterSubstitutional++;
        } else {
            xMSumProduct += xSumSubstitutional * pEl->mvU * (mvpMajorElement->mvMolarMass - pEl->mvMolarMass);
        }
    }

    // If the composition has changed
    if (notUpdatedCounterInterstitial + notUpdatedCounterSubstitutional > 0) {
        // Evaluates average molar mass
        mvMolarMassAvg = mvpMajorElement->mvMolarMass - xMSumProduct - xSumSubstitutional * mvMolarMassAvgFixedPartial;

        // If interstitial element fraction changed, then updates site fractions of variable interstitial elements
        if (notUpdatedCounterInterstitial > 0) {
            for (ElementPointer pEl : mvVariableInterstitialElements) {
                if (!pEl->mvIsUpdated) {
                    pEl->mvU = pEl->mvX / xSumSubstitutional;
                }
            }
        }

        // Updates site fractions of variable substitutional elements
        for (ElementPointer pEl : mvVariableSubstitutionalElements) {
            if (!pEl->mvIsUpdated) {
                pEl->mvU = pEl->mvX / xSumSubstitutional;
            }
        }

        // Updates mole and mass fractions of all alloying elements
        for (ElementPointer pEl : mvAlloyingElements) {
            if (notUpdatedCounterInterstitial > 0) {
                if (pEl->mvIsUpdated)
                    pEl->mvX = pEl->mvU * xSumSubstitutional;
                else
                    pEl->mvIsUpdated = true;
            }
            pEl->mvW = pEl->mvX * pEl->mvMolarMass / mvMolarMassAvg;
            xSumAlloying += pEl->mvX;
        }

        mvpMajorElement->mvX = 1.0 - xSumAlloying;
        mvpMajorElement->mvW = mvpMajorElement->mvX * mvpMajorElement->mvMolarMass / mvMolarMassAvg;
        mvpMajorElement->mvU = mvpMajorElement->mvX / xSumSubstitutional;
    }
}