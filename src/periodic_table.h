/// @file src/periodic_table.h

#ifndef PERIODIC_TABLE_H
#define PERIODIC_TABLE_H

#include <string>

namespace PeriodicTable {
/// Simple struct to store an element basic data
struct Element {
    std::string Symbol; ///< The symbol
    std::string Name; ///< The name
    unsigned int AtomicNumber; ///< The atomic number
    double MolarMass; ///< The molar mass
};

extern const Element H; ///< Hydrogen
extern const Element He; ///< Helium
extern const Element Li; ///< Lithium
extern const Element Be; ///< Beryllium
extern const Element B; ///< Boron
extern const Element C; ///< Carbon
extern const Element N; ///< Nitrogen
extern const Element O; ///< Oxygen
extern const Element F; ///< Fluorine
extern const Element Ne; ///< Neon
extern const Element Na; ///< Sodium
extern const Element Mg; ///< Magnesium
extern const Element Al; ///< Aluminum
extern const Element Si; ///< Silicon
extern const Element P; ///< Phosphorus
extern const Element S; ///< Sulfur
extern const Element Cl; ///< Chlorine
extern const Element Ar; ///< Argon
extern const Element K; ///< Potassium
extern const Element Ca; ///< Calcium
extern const Element Sc; ///< Scandium
extern const Element Ti; ///< Titanium
extern const Element V; ///< Vanadium
extern const Element Cr; ///< Chromium
extern const Element Mn; ///< Manganese
extern const Element Fe; ///< Iron
extern const Element Co; ///< Cobalt
extern const Element Ni; ///< Nickel
extern const Element Cu; ///< Copper
extern const Element Zn; ///< Zinc
extern const Element Ga; ///< Gallium
extern const Element Ge; ///< Germanium
extern const Element As; ///< Arsenic
extern const Element Se; ///< Selenium
extern const Element Br; ///< Bromine
extern const Element Kr; ///< Krypton
extern const Element Rb; ///< Rubidium
extern const Element Sr; ///< Strontium
extern const Element Y; ///< Yttrium
extern const Element Zr; ///< Zirconium
extern const Element Nb; ///< Niobium
extern const Element Mo; ///< Molybdenum
extern const Element Tc; ///< Technetium
extern const Element Ru; ///< Ruthenium
extern const Element Rh; ///< Rhodium
extern const Element Pd; ///< Palladium
extern const Element Ag; ///< Silver
extern const Element Cd; ///< Cadmium
extern const Element In; ///< Indium
extern const Element Sn; ///< Tin
extern const Element Sb; ///< Antimony
extern const Element Te; ///< Tellurium
extern const Element I; ///< Iodine
extern const Element Xe; ///< Xenon
extern const Element Cs; ///< Cesium
extern const Element Ba; ///< Barium
extern const Element La; ///< Lanthanum
extern const Element Ce; ///< Cerium
extern const Element Pr; ///< Praseodymium
extern const Element Nd; ///< Neodymium
extern const Element Pm; ///< Promethium
extern const Element Sm; ///< Samarium
extern const Element Eu; ///< Europium
extern const Element Gd; ///< Gadolinium
extern const Element Tb; ///< Terbium
extern const Element Dy; ///< Dysprosium
extern const Element Ho; ///< Holmium
extern const Element Er; ///< Erbium
extern const Element Tm; ///< Thulium
extern const Element Yb; ///< Ytterbium
extern const Element Lu; ///< Lutetium
extern const Element Hf; ///< Hafnium
extern const Element Ta; ///< Tantalum
extern const Element W; ///< Tungsten
extern const Element Re; ///< Rhenium
extern const Element Os; ///< Osmium
extern const Element Ir; ///< Iridium
extern const Element Pt; ///< Platinum
extern const Element Au; ///< Gold
extern const Element Hg; ///< Mercury
extern const Element Tl; ///< Thallium
extern const Element Pb; ///< Lead
extern const Element Bi; ///< Bismuth
extern const Element Po; ///< Polonium
extern const Element At; ///< Astatine
extern const Element Rn; ///< Radon
extern const Element Fr; ///< Francium
extern const Element Ra; ///< Radium
extern const Element Ac; ///< Actinium
extern const Element Th; ///< Thorium
extern const Element Pa; ///< Protactinium
extern const Element U; ///< Uranium
extern const Element Np; ///< Neptunium
extern const Element Pu; ///< Plutonium
extern const Element Am; ///< Americium
extern const Element Cm; ///< Curium
extern const Element Bk; ///< Berkelium
extern const Element Cf; ///< Californium
extern const Element Es; ///< Einsteinium
extern const Element Fm; ///< Fermium
extern const Element Md; ///< Mendelevium
extern const Element No; ///< Nobelium
extern const Element Lr; ///< Lawrencium
extern const Element Rf; ///< Rutherfordium
extern const Element Db; ///< Dubnium
extern const Element Sg; ///< Seaborgium
extern const Element Bh; ///< Bohrium
extern const Element Hs; ///< Hassium
extern const Element Mt; ///< Meitnerium
extern const Element Ds; ///< Darmstadtium
extern const Element Rg; ///< Roentgenium
extern const Element Cn; ///< Copernicium
extern const Element Nh; ///< Nihonium
extern const Element Fl; ///< Flerovium
extern const Element Mc; ///< Moscovium
extern const Element Lv; ///< Livermorium
extern const Element Ts; ///< Tennessine
extern const Element Og; ///< Oganesson
}

#endif