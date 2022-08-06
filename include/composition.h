/// @file src/composition.h

#ifndef COMPOSITION_H
#define COMPOSITION_H

#include "periodic_table.h"
#include <map>
#include <string>
#include <vector>

/// @brief Class with properties of a single element in the alloy (molar mass, fractions, etc)
class ElementData {
private:
    std::string mvSymbol = "undefined"; ///< %Element symbol
    bool mvIsMajor = false; ///< If it is major element in Composition
    bool mvIsInterstitial = false; ///< True if it is interstitial element, false if it is substitutional
    bool mvIsVariable = true; ///< If composition of element is allowed to be changed in Composition even when composition is locked
    bool mvIsAllowedToVary = true; ///< Flag used to temporarily allow composition of element to change when composition is unlocked
    bool mvIsUpdated = false; ///< If composition of element is updated in Composition
    bool mvIsCompositionLocked = false; ///< If Composition is locked
    double mvMolarMass = 0.0; ///< Molar mass
    double mvUserX = 0.0; ///< User defined mole fraction
    double mvUserW = 0.0; ///< User defined mass fraction
    double mvX = 0.0; ///< Calculated mole fraction
    double mvW = 0.0; ///< Calculated mass fraction
    double mvU = 0.0; ///< Calculated site fraction

public:
    /// Default constructor
    ElementData() { }
    /// Constructor of ElementData
    ElementData(const PeriodicTable::Element& element, bool isInterstitial = false, bool isVariable = false, bool isMajor = false);

    /// @name Setters
    /// @{
    /// Set mole fraction
    void SetX(double x);

    /// Set weight fraction
    void SetW(double w);
    /// @}

    /// @name Getters
    /// @{
    /// Get element symbol
    std::string GetSymbol() const { return mvSymbol; }
    /// Get molar mass of element
    double GetMolarMass() const { return mvMolarMass; }

    /// Get mole fraction
    double GetX() const { return mvX; }
    /// Get weight fraction
    double GetW() const { return mvW; }
    /// Get U-fraction (site fraction)
    double GetU() const { return mvU; }

    /// Whether is major element
    bool IsMajor() const { return mvIsMajor; }
    /// Whether is interstitial element
    bool IsInterstitial() const { return mvIsInterstitial; }
    /// Whether is variable element
    bool IsVariable() const { return mvIsVariable; }
    /// @}

    friend class CompositionBase;
    friend class Composition;
};

/// const ElementData
typedef const ElementData ConstElementData;
/// Pointer to a defined element
typedef ElementData* ElementPointer;
/// Vector of pointers to defined elements
typedef std::vector<ElementPointer> VectorElementPointers;
/// Const pointer to a defined element
typedef const ElementData* ConstElementPointer;
/// Vector of const pointers to defined elements
typedef std::vector<ConstElementPointer> VectorConstElementPointers;

/// Iterable container of elements. It can be used with the range-based for syntax
template <typename T>
class PointersContainer {
private:
    std::vector<T*> mvElements; ///< Elements

public:
    /// Iterator for a container of pointers
    class PointersIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        PointersIterator(pointer* ptrPtr)
            : mvpPtrPtr(ptrPtr)
        {
        }

        reference operator*() const { return **mvpPtrPtr; }
        pointer operator->() { return *mvpPtrPtr; }
        PointersIterator& operator++()
        {
            ++mvpPtrPtr;
            return *this;
        }
        PointersIterator operator++(int)
        {
            PointersIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const PointersIterator& a, const PointersIterator& b) { return a.mvpPtrPtr == b.mvpPtrPtr; };
        friend bool operator!=(const PointersIterator& a, const PointersIterator& b) { return a.mvpPtrPtr != b.mvpPtrPtr; };

    private:
        pointer* mvpPtrPtr; ///< Pointer to element of the container (which is also a pointer!)
    };

    /// Constructor
    PointersContainer(const std::vector<T*>& elements)
        : mvElements(elements)
    {
    }

    /// Begin of the container
    PointersIterator begin() { return &*mvElements.begin(); }
    /// End of the container
    PointersIterator end() { return &*mvElements.end(); }
};

/// Container of pointers to ElementData
typedef PointersContainer<ElementData> ContainerElements;
/// Container of pointers to const ElementData
typedef PointersContainer<const ElementData> ContainerConstElements;

/** @brief Base class to Composition with pointers to the the elements
 * 
 * The pointers are used here to be able to dynamically loop through
 * all defined elements (ElementData), without having to make use of 
 * maps or copies of the elements. The pointers are non-copyable or
 * moveable because they should refer exclusively to the element data
 * instantiated with an instance of the Composition class. Otherwise, 
 * we could have pointers to elements from other instances.
 */
class CompositionBase {
public:
protected:
    ElementPointer mvpMajorElement = nullptr; ///< Pointer to major element (e.g., Fe)

    VectorElementPointers mvAlloyingElements; ///< Vector of pointers to all alloying elements

    VectorElementPointers mvInterstitialElements; ///< Vector of pointers to interstitial elements
    VectorElementPointers mvFixedInterstitialElements; ///< Vector of pointers to fixed interstitial elements
    VectorElementPointers mvVariableInterstitialElements; ///< Vector of pointers to interstitial elements that can be changed

    VectorElementPointers mvSubstitutionalElements; ///< Vector of pointers to substitutional elements
    VectorElementPointers mvFixedSubstitutionalElements; ///< Vector of pointers to fixed substitutional elements
    VectorElementPointers mvVariableSubstitutionalElements; ///< Vector of pointers to substitutional elements that can be changed

    VectorElementPointers mvVariableElements; ///< Vector of pointers to all elements that can be changed
    VectorElementPointers mvFixedElements; ///< Vector of pointers to all fixed elements

    bool mvArePointersUpdated = false; ///< If true, element definitions are not updated

protected:
    void updatePointers();
    /// Returns a vector with pointers to all defined elements
    virtual VectorElementPointers getElementPointers() = 0;
    /// Returns a vector with const pointers to all defined elements
    virtual VectorConstElementPointers getElementPointers() const = 0;

public:
    /// Default constructor
    CompositionBase() { }
    /// Destructor
    virtual ~CompositionBase() { }
    /// Copy constructor. Does nothing! We don't want to copy the pointers to
    /// the elements. Always call updatePointers after instantiating/copying/moving
    /// Composition
    CompositionBase(const CompositionBase&) { }
    /// Copy assignment operator. Does nothing! We don't want to copy the pointers to
    /// the elements. Always call updatePointers after instantiating/copying/moving
    /// Composition
    CompositionBase& operator=(const CompositionBase&) { return *this; }
    /// Move constructor. Does nothing! We don't want to copy the pointers to
    /// the elements. Always call updatePointers after instantiating/copying/moving
    /// Composition
    CompositionBase(const CompositionBase&&) { }
    /// Move assignment operator. Does nothing! We don't want to copy the pointers to
    /// the elements. Always call updatePointers after instantiating/copying/moving
    /// Composition
    CompositionBase& operator=(const CompositionBase&&) { return *this; }

    ElementData& operator[](const std::string& elementSymbol);
    const ElementData& operator[](const std::string& elementSymbol) const;

    /// Gets the symbol of the major element
    const std::string GetMajorElementSymbol() const { return mvpMajorElement->mvSymbol; }

    /// Returns an iterable container with all defined elements. One can can loop through
    /// all elements using the range-based for syntax:
    ContainerElements GetElements() { return ContainerElements(getElementPointers()); }
    ContainerConstElements GetElements() const { return ContainerConstElements(getElementPointers()); }
};

/** @brief Class used for composition base conversions: atomic <-> mass 
 * fractions
 *
 * Note: while the performance of the composition conversion in this class
 * is quite reasonable, copying its instances is relatively slow.
 */
class Composition : public CompositionBase {
private:
    bool mvIsCompositionLocked = false; ///< If true, only the fractions of the variable elements (mvVariableElements) can be changed
    double mvMolarMassAvg = 0.0; ///< Average molar mass
    double mvMolarMassAvgFixedPartial = 0.0; ///< Fixed partial component of the molar mass
    double mvXSumSubstitutionalFixedPartial = 0.0; ///< Fixed partial component of the fraction of substitutional elements

    void updateFractions();
    void updateFractionsUFixed();

public:
    /// Default constructor
    Composition() { }

    /// Returns if composition is locked
    bool IsCompositionLocked() const { return mvIsCompositionLocked; }

    void LockComposition();
    void UnlockComposition();
    void UpdateFractions();
    void Print(FILE* stream = stdout);
    void Print(FILE* stream = stdout) const;
};

/// Used together with FOR_ELEMENTS in MAKE_DEFINITIONS. Takes the element
/// symbol and defines and initializes ElementData using the PeriodicTable
#define DEFINE_ELEMENT(element, ...) ElementData element = ElementData(PeriodicTable::element, ##__VA_ARGS__);

/// Used together with FOR_ELEMENTS in MAKE_DEFINITIONS. Append the pointer
/// of each element to concatenate them together
#define APPEND_ELEMENT_POINTER(element, ...) &element,

/// Defines all elements and necessary virtual functions from Composition and CompositionBase
#define MAKE_DEFINITIONS(FOR_ELEMENTS)                                                              \
public:                                                                                             \
    /* Define elements (ElementData) as public members */                                           \
    FOR_ELEMENTS(DEFINE_ELEMENT)                                                                    \
private:                                                                                            \
    /* Override virtual functions */                                                                \
    VectorElementPointers getElementPointers() { return { FOR_ELEMENTS(APPEND_ELEMENT_POINTER) }; } \
    VectorConstElementPointers getElementPointers() const { return { FOR_ELEMENTS(APPEND_ELEMENT_POINTER) }; }

/** @brief Make a composition class for a given set of elements
 * 
 * The macro takes two arguments, the name of the class ClassName and the
 * macro FOR_ELEMENTS. In FOR_ELEMENTS are defined and used in the 
 * following maner:
 * 
 * @code{.cpp}
 * #define FOR_STEEL_ELEMENTS(DO) \
 *     DO(Fe, false, false, true) \
 *     DO(C, true, true)          \
 *     DO(Mn, false, true)        \
 *     DO(Si)
 *     
 * MAKE_COMPOSITION_CLASS(CompositionSteel, FOR_STEEL_ELEMENTS)
 * @endcode
 * 
 * The DO notation in FOR_STEEL_ELEMENTS is used due to the X Macro
 * technique used for making this class dynamic (see https://en.wikipedia.org/wiki/X_Macro)
 * The arguments in the DO macro call follow the Constructor of ElementData,
 * i.e., are respectively the element symbol, isInterstitial, isVariable, and
 * isMajor.
 */
#define MAKE_COMPOSITION_CLASS(ClassName, FOR_ELEMENTS) \
    class ClassName : public Composition {              \
        /* Define elements and virtual functions */     \
        MAKE_DEFINITIONS(FOR_ELEMENTS)                  \
    public:                                             \
        /* Constructor */                               \
        ClassName()                                     \
            : Composition()                             \
        {                                               \
            updatePointers();                           \
        }                                               \
    };

#endif