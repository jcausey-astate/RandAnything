/**
 * Just a few simple tests for RandAnything,
 * showing that it can generate different kinds of
 * random values with almost no setup.
 */
#include <iostream>
#include <iomanip>
#include "../RandAnything.h"
    
int main() {
    RandAnything<unsigned> ra1;
    RandAnything<double> ra2;
    RandAnything<long double> ra3;
    RandAnything<std::string> strand;
    
    std::cout << "Integers (unsigned): \n";    
    for(int i = 0; i < 60; ++i){
        std::cout << std::setw(6) << std::left
                  << ra1(1,6) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";
    std::cout << "Floating-Point (double): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::fixed << std::setprecision(3) << std::setw(6)
                  << ra2(1,6) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";
    std::cout << "Floating-Point (double, [0,1)): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::fixed << std::setprecision(3) << std::setw(6)
                  << ra2(0,1) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";
    std::cout << "Floating-Point (long double): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::fixed << std::setprecision(3) << std::setw(6)
                  << ra3(1,6) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";
    std::cout << "String (length [2,5], alpha-numeric): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::setw(6) 
                  << strand(2,5,strand.alphabet_alphaNumeric()) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";
    std::cout << "String (length 5, alphabetical, with upper- and lower-case): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::setw(6) 
                  << strand(5,strand.alphabet_alphaAllCase()) << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";    
    return 0;
}
