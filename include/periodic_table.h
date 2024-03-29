/// @file periodic_table.h

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

const Element H { "H", "Hydrogen", 1, 1.00794 };
const Element He { "He", "Helium", 2, 4.002602 };
const Element Li { "Li", "Lithium", 3, 6.941 };
const Element Be { "Be", "Beryllium", 4, 9.012182 };
const Element B { "B", "Boron", 5, 10.811 };
const Element C { "C", "Carbon", 6, 12.0107 };
const Element N { "N", "Nitrogen", 7, 14.0067 };
const Element O { "O", "Oxygen", 8, 15.9994 };
const Element F { "F", "Fluorine", 9, 18.9984032 };
const Element Ne { "Ne", "Neon", 10, 20.1797 };
const Element Na { "Na", "Sodium", 11, 22.98977 };
const Element Mg { "Mg", "Magnesium", 12, 24.305 };
const Element Al { "Al", "Aluminum", 13, 26.981538 };
const Element Si { "Si", "Silicon", 14, 28.0855 };
const Element P { "P", "Phosphorus", 15, 30.973761 };
const Element S { "S", "Sulfur", 16, 32.065 };
const Element Cl { "Cl", "Chlorine", 17, 35.453 };
const Element Ar { "Ar", "Argon", 18, 39.948 };
const Element K { "K", "Potassium", 19, 39.0983 };
const Element Ca { "Ca", "Calcium", 20, 40.078 };
const Element Sc { "Sc", "Scandium", 21, 44.95591 };
const Element Ti { "Ti", "Titanium", 22, 47.867 };
const Element V { "V", "Vanadium", 23, 50.9415 };
const Element Cr { "Cr", "Chromium", 24, 51.9961 };
const Element Mn { "Mn", "Manganese", 25, 54.938049 };
const Element Fe { "Fe", "Iron", 26, 55.845 };
const Element Co { "Co", "Cobalt", 27, 58.9332 };
const Element Ni { "Ni", "Nickel", 28, 58.6934 };
const Element Cu { "Cu", "Copper", 29, 63.546 };
const Element Zn { "Zn", "Zinc", 30, 65.409 };
const Element Ga { "Ga", "Gallium", 31, 69.723 };
const Element Ge { "Ge", "Germanium", 32, 72.64 };
const Element As { "As", "Arsenic", 33, 74.9216 };
const Element Se { "Se", "Selenium", 34, 78.96 };
const Element Br { "Br", "Bromine", 35, 79.904 };
const Element Kr { "Kr", "Krypton", 36, 83.798 };
const Element Rb { "Rb", "Rubidium", 37, 85.4678 };
const Element Sr { "Sr", "Strontium", 38, 87.62 };
const Element Y { "Y", "Yttrium", 39, 88.90585 };
const Element Zr { "Zr", "Zirconium", 40, 91.224 };
const Element Nb { "Nb", "Niobium", 41, 92.90638 };
const Element Mo { "Mo", "Molybdenum", 42, 95.94 };
const Element Tc { "Tc", "Technetium", 43, 98 };
const Element Ru { "Ru", "Ruthenium", 44, 101.07 };
const Element Rh { "Rh", "Rhodium", 45, 102.9055 };
const Element Pd { "Pd", "Palladium", 46, 106.42 };
const Element Ag { "Ag", "Silver", 47, 107.8682 };
const Element Cd { "Cd", "Cadmium", 48, 112.411 };
const Element In { "In", "Indium", 49, 114.818 };
const Element Sn { "Sn", "Tin", 50, 118.71 };
const Element Sb { "Sb", "Antimony", 51, 121.76 };
const Element Te { "Te", "Tellurium", 52, 127.6 };
const Element I { "I", "Iodine", 53, 126.90447 };
const Element Xe { "Xe", "Xenon", 54, 131.293 };
const Element Cs { "Cs", "Cesium", 55, 132.90545 };
const Element Ba { "Ba", "Barium", 56, 137.327 };
const Element La { "La", "Lanthanum", 57, 138.9055 };
const Element Ce { "Ce", "Cerium", 58, 140.116 };
const Element Pr { "Pr", "Praseodymium", 59, 140.90765 };
const Element Nd { "Nd", "Neodymium", 60, 144.24 };
const Element Pm { "Pm", "Promethium", 61, 145 };
const Element Sm { "Sm", "Samarium", 62, 150.36 };
const Element Eu { "Eu", "Europium", 63, 151.964 };
const Element Gd { "Gd", "Gadolinium", 64, 157.25 };
const Element Tb { "Tb", "Terbium", 65, 158.92534 };
const Element Dy { "Dy", "Dysprosium", 66, 162.5 };
const Element Ho { "Ho", "Holmium", 67, 164.93032 };
const Element Er { "Er", "Erbium", 68, 167.259 };
const Element Tm { "Tm", "Thulium", 69, 168.93421 };
const Element Yb { "Yb", "Ytterbium", 70, 173.04 };
const Element Lu { "Lu", "Lutetium", 71, 174.967 };
const Element Hf { "Hf", "Hafnium", 72, 178.49 };
const Element Ta { "Ta", "Tantalum", 73, 180.9479 };
const Element W { "W", "Tungsten", 74, 183.84 };
const Element Re { "Re", "Rhenium", 75, 186.207 };
const Element Os { "Os", "Osmium", 76, 190.23 };
const Element Ir { "Ir", "Iridium", 77, 192.217 };
const Element Pt { "Pt", "Platinum", 78, 195.078 };
const Element Au { "Au", "Gold", 79, 196.96655 };
const Element Hg { "Hg", "Mercury", 80, 200.59 };
const Element Tl { "Tl", "Thallium", 81, 204.3833 };
const Element Pb { "Pb", "Lead", 82, 207.2 };
const Element Bi { "Bi", "Bismuth", 83, 208.98038 };
const Element Po { "Po", "Polonium", 84, 209 };
const Element At { "At", "Astatine", 85, 210 };
const Element Rn { "Rn", "Radon", 86, 222 };
const Element Fr { "Fr", "Francium", 87, 223 };
const Element Ra { "Ra", "Radium", 88, 226 };
const Element Ac { "Ac", "Actinium", 89, 227 };
const Element Th { "Th", "Thorium", 90, 232.0381 };
const Element Pa { "Pa", "Protactinium", 91, 231.03588 };
const Element U { "U", "Uranium", 92, 238.02891 };
const Element Np { "Np", "Neptunium", 93, 237 };
const Element Pu { "Pu", "Plutonium", 94, 244 };
const Element Am { "Am", "Americium", 95, 243 };
const Element Cm { "Cm", "Curium", 96, 247 };
const Element Bk { "Bk", "Berkelium", 97, 247 };
const Element Cf { "Cf", "Californium", 98, 251 };
const Element Es { "Es", "Einsteinium", 99, 252 };
const Element Fm { "Fm", "Fermium", 100, 257 };
const Element Md { "Md", "Mendelevium", 101, 258 };
const Element No { "No", "Nobelium", 102, 259 };
const Element Lr { "Lr", "Lawrencium", 103, 262 };
const Element Rf { "Rf", "Rutherfordium", 104, 261 };
const Element Db { "Db", "Dubnium", 105, 262 };
const Element Sg { "Sg", "Seaborgium", 106, 266 };
const Element Bh { "Bh", "Bohrium", 107, 264 };
const Element Hs { "Hs", "Hassium", 108, 277 };
const Element Mt { "Mt", "Meitnerium", 109, 268 };
const Element Ds { "Ds", "Darmstadtium", 110, 281 };
const Element Rg { "Rg", "Roentgenium", 111, 272 };
const Element Cn { "Cn", "Copernicium", 112, 285 };
const Element Nh { "Nh", "Nihonium", 113, 286 };
const Element Fl { "Fl", "Flerovium", 114, 289 };
const Element Mc { "Mc", "Moscovium", 115, 289 };
const Element Lv { "Lv", "Livermorium", 116, 293 };
const Element Ts { "Ts", "Tennessine", 117, 294 };
const Element Og { "Og", "Oganesson", 118, 294 };
}

#endif