/**
 * Just a few simple tests for RandAnything,
 * showing that it can generate different kinds of
 * random values with almost no setup.
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include "../RandAnything.h"

class Foo {};

    
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
    std::cout << "String (length 4, simulated binary): \n";
    for(int i = 0; i < 60; ++i){
        std::cout << std::setw(6) 
                  << strand(4,"01") << ((i+1)%10 == 0 ? "\n" : "");
    }
    std::cout << "\n\n";    

    // Un-comment the following line to test type compatibility check (the code
    // should not compile with the line un-commented):
    // RandAnything<Foo> foorand;  // type compatibility check (should not compile if active)
    
    // Tests fixed seeds:
    std::cout << "Fixed seed verification:\n";
    RandAnything<int>    fixed1 {33};
    RandAnything<double> dfixed1{33};
    std::vector<int>     v;
    std::vector<double>  dv;
    for(int i = 0; i < 10; ++i){
        v.push_back(fixed1(1, 1000));
        dv.push_back(dfixed1(0, 100));
     }
    RandAnything<int>    fixed2 {33};
    RandAnything<double> dfixed2{33};
    for(int i = 0; i < 10; ++i){
        auto item    = v [i];
        auto ditem   = dv[i];
        auto verify  = fixed2 (1, 1000);
        auto dverify = dfixed2(0, 100 );
        if(item != verify || ditem != dverify){
            std::cout << "FAILED fixed seed test. " << item    << " != " << verify 
                      << " or " << ditem << " != "  << dverify << std::endl;
            i = 10;
        }
        else{
            std::cout << std::setprecision(3) << std::fixed << std::right
                      << std::setw(4) << item  << " == " << std::setw(4) << verify  << " -- "
                      << std::setw(6) << ditem << " == " << std::setw(6) << dverify << "\n";
        }
    }
    
    std::cout << "\n\n";
    
    std::cout << "Done.\n";    
    return 0;
}
