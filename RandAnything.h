#ifndef RANDANYTHING_H
#define RANDANYTHING_H
/**
 * @file RandAnything.h
 * Defines a class RandAnything that will allow you to quickly
 * generate a quality pseudo-random value of (almost) any 
 * standard type without worrying about STL type names or 
 * doing a lot of setup.
 * 
 * @copyright 2015 Jason L Causey, Arkansas State University
 *                                 Department of Computer Science
 * 
 *  The MIT License (MIT)  http://opensource.org/licenses/MIT 
 *  
 *  Copyright (c) 2015 Jason L Causey, Arkansas State University
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.     
 */
#include <functional>
#include <memory>
#include <random>
#include <type_traits>

template <typename ValueType> class _RandHelper;
template <typename ValueType> class _IntRandHelper;
template <typename ValueType> class _RealRandHelper;
class _StringRandHelper;

/**
 * @brief   Generate a random value of any numeric type or std::string.
 * @details Generate (almost) any type of uniform random value in a range [low,high]
 *          (for integral values) or [low, high) (for floating-point values).
 *          Just instantiate the class with whatever type you want as the template
 *          argument, then use it as a function where the arguments are the 
 *          lower and upper bounds of the range for the resulting random value.
 *          To generate `std::string` values, @seealso RandAnything<std::string> 
 *          specialization.
 * 
 * @tparam ValueType Type of value that should be generated.  Supports integral types,
 *                   Real-number types, and `std::string`.
 */
template <typename ValueType>
class RandAnything{
public:
    RandAnything();
    ValueType operator() (const ValueType& low, const ValueType& high) const;
private:
    std::random_device                          seeder;
    std::mt19937                                generator{seeder()};
    std::unique_ptr< _RandHelper<ValueType> >   rh;
    RandAnything  (const RandAnything&)         = delete;
    void operator=(const RandAnything&)         = delete;
};

/**
 * constructs the random number generator and prepares it for use (seeding included)
 * 
 * @tparam ValueType Type of value that should be generated.  Supports integral types,
 *                   Real-number types, and `std::string`.
 */
template <typename ValueType>
RandAnything<ValueType>::RandAnything(){
    if(std::is_integral<ValueType>::value){
        rh = std::unique_ptr< _RandHelper<ValueType> >{new _IntRandHelper<ValueType>{&generator}};
    }
    else if(std::is_floating_point<ValueType>::value){
        rh = std::unique_ptr< _RandHelper<ValueType> >{new _RealRandHelper<ValueType>{&generator}};
    }
    else{
        auto t_name = typeid(ValueType).name();
        throw std::runtime_error(std::string{"Unable to generate randoms of type "} + t_name);
    }
}

/**
 * Generate random value in range [low,high].
 * 
 * @param low  smallest value that can be generated
 * @param high largest value that can be generated
 * 
 * @tparam ValueType Type of value that should be generated.  Supports integral types,
 *                   Real-number types, and `std::string`.
 * @return a (uniform) random number in the range [low, high]
 */
template <typename ValueType>
ValueType RandAnything<ValueType>::operator() (const ValueType& low, const ValueType& high) const{
    return rh->generate(low, high);
}

/*
 * Virtual base-class to encapsulate the different types of numeric random-number generators
 * from the <random> library, allowing polymorphic behavior in the `RandAnything` class.
 */
template <typename ValueType>
class _RandHelper{
public:
    _RandHelper(std::mt19937* gen) : generator{gen} { }
    virtual ValueType generate (const ValueType& low, const ValueType& high) const = 0;
    virtual ~_RandHelper() = default;
protected:
    std::mt19937* generator;
};

/*
 * Helper sub-class for generating integral values.
 */
template <typename ValueType>
class _IntRandHelper : public _RandHelper<ValueType>{
public:
    _IntRandHelper(std::mt19937* gen) : _RandHelper<ValueType>{gen} {  }
    virtual ValueType generate (const ValueType& low, const ValueType& high) const;
};

/*
 * Generate an integral value in the range [low,high] using the std::uniform_int_distribution
 */
template <typename ValueType>
ValueType _IntRandHelper<ValueType>::generate (const ValueType& low, const ValueType& high) const{
    std::uniform_int_distribution<ValueType> distribution{low, high};
    return distribution(*this->generator);
}

/*
 * Helper sub-class for generating floating_point values.
 */
template <typename ValueType>
class _RealRandHelper : public _RandHelper<ValueType>{
public:
    _RealRandHelper(std::mt19937* gen) : _RandHelper<ValueType>{gen} {  }
    virtual ValueType generate (const ValueType& low, const ValueType& high) const;
};

/*
 * Generate a floating-point value in the range [low,high] using the std::uniform_int_distribution
 */
template <typename ValueType>
ValueType _RealRandHelper<ValueType>::generate (const ValueType& low, const ValueType& high) const{
    std::uniform_real_distribution<ValueType> distribution{low, high};
    return distribution(*this->generator);
}

/*
 * Helper class for generating std::string values (note: not a sub-class of `_RandHelper`)
 * This one is a stand-alone class since the concept of the `low` and `high` range limits
 * being the same type of data as what is returned does not apply.
 */
class _StringRandHelper{
public:
    _StringRandHelper(std::mt19937* gen) : generator{gen} {  }
    virtual std::string generate (int length, std::string alphabet) const;
    virtual std::string generate (int low, int high, std::string alphabet) const;
protected:
    std::mt19937*     generator;
    RandAnything<int> rnd;
};
/*
 * Generate a `std::string` value of length `length`, chosen from `alphabet`
 */
std::string _StringRandHelper::generate (int length, std::string alphabet) const{
    return generate(length, length, alphabet);
}
/*
 * Generate a `std::string` value with a length in the range [`low`,`high`], chosen from `alphabet`
 */
std::string _StringRandHelper::generate (int low, int high, std::string alphabet) const{
    int length = rnd(low, high);
    std::string result;
    for(int i = 0; i < length; ++i){                      // generate `length` random characters
        result += alphabet[ rnd(0, alphabet.size()-1) ];  // from within the desired alphabet
    }
    return result;                                        // return the resulting random string
}

/**
 * @brief   RandAnything specialization for std::string generation
 * @details Generates std::strings with either a fixed length or with a range of lengths 
 *          given an alphabet of characters to choose from (or using all printable characters).
 *          This class also exposes methods to generate several useful alphabets.
 *
 * @tparam   [description]
 */
template < >
class RandAnything<std::string>{
public:
    RandAnything() = default;
    std::string operator() (int length, std::string alphabet="") const;
    std::string operator() (int min_length, int max_length, std::string alphabet="") const;
    std::string alphabet_printable()const;
    std::string alphabet_alphaAllCase()const;
    std::string alphabet_alphaLowerCase()const;
    std::string alphabet_alphaUpperCase()const;
    std::string alphabet_alphaNumeric()const;
    std::string alphabet_numeric()const;
    std::string alphabet_punctuation()const;
    std::string alphabet_hexadecimal()const;
private:
    std::random_device      seeder;
    std::mt19937            generator{seeder()};
    _StringRandHelper       rh{&generator};
    RandAnything(const RandAnything&)   = delete;
    void operator=(const RandAnything&) = delete;
};

/**
 * @brief   generate a random `std::string` of a specific length from a chosen alphabet
 * 
 * @details Generates a `std::string` of characters containing characters chosen at 
 *          random from `alphabet` (uniform choice, with replacement).  The length of 
 *          the generated string is given by `length`.
 * 
 * @param length     length of generated string
 * @param alphabet   set of characters that may appear in the generated string
 * @return  a random string of characters from `alphabet` whose length 
 *          is given by `length`
 */
inline std::string RandAnything<std::string>::operator() (int length, std::string alphabet) const{ 
    return (*this)(length, length, alphabet); 
}

/**
 * @brief   generate a random `std::string` in a range of lengths from a chosen alphabet
 * 
 * @details Generates a `std::string` of characters containing characters chosen at 
 *          random from `alphabet` (uniform choice, with replacement).  The minimum and maximum
 *          possible lengths for the generated string are given by `min_length` and `max_length`,
 *          respectively.
 * 
 * @param min_length shortest possible string to generate   
 * @param max_length longest possible string to generate
 * @param alphabet   set of characters that may appear in the generated string
 * @return  a random string of characters from `alphabet` whose length 
 *          is in the range [`min_length`, `max_length`]
 */
std::string RandAnything<std::string>::operator() (int min_length, int max_length, std::string alphabet) const{
    if(alphabet.size() == 0)                              // if the alphabet is blank, use all printable chars 
        alphabet = alphabet_printable();                           
    return rh.generate(min_length, max_length, alphabet);
}

/**
 * @brief   generates the alphabet of all lowercase alphabetical characters
 * @return  a `std::string` consisting of all lowercase alphabetical characters
 */
std::string RandAnything<std::string>::alphabet_alphaLowerCase()const{
    std::string result;
    for(char ch = 'a'; ch <= 'z'; ++ch)
        result += ch;
    return result;
}
/**
 * @brief   generates the alphabet of all uppercase alphabetical characters
 * @return  a `std::string` consisting of all uppercase alphabetical characters
 */
std::string RandAnything<std::string>::alphabet_alphaUpperCase()const{
    std::string result;
    for(char ch = 'A'; ch <= 'Z'; ++ch)
        result += ch;
    return result;
}
/**
 * @brief   generates the alphabet of all numeric characters
 * @return  a `std::string` consisting of all numeric characters
 */
std::string RandAnything<std::string>::alphabet_numeric()const{
    std::string result;
    for(char ch = '0'; ch <= '9'; ++ch)
        result += ch;
    return result;
}
/**
 * @brief   generates the alphabet of all alphabetical characters (upper- and lower-case)
 * @return  a `std::string` consisting of all alphabetical characters
 */
std::string RandAnything<std::string>::alphabet_alphaAllCase()const{
    return alphabet_alphaLowerCase() + alphabet_alphaUpperCase();
}
/**
 * @brief   generates the alphabet of all alphabetical (upper- and lower-case) and numeric digits
 * @return  a `std::string` consisting of all alphabetical characters and digits
 */
std::string RandAnything<std::string>::alphabet_alphaNumeric()const{
    return alphabet_alphaLowerCase() + alphabet_alphaUpperCase() + alphabet_numeric();
}
/**
 * @brief   generates the alphabet of all punctuation and symbol characters 
 *          (all non-whitespace printable characters that are not alphabetical or numeric)
 * @return  a `std::string` consisting of all punctuation and symbol printable characters
 */
std::string RandAnything<std::string>::alphabet_punctuation()const{
    std::string result;
    for(char ch = '!'; ch <= '/'; ++ch)
        result += ch;
    for(char ch = ':'; ch <= '@'; ++ch)
        result += ch;
    for(char ch = '['; ch <= '`'; ++ch)
        result += ch;
    for(char ch = '{'; ch <= '~'; ++ch)
        result += ch;
    return result;
}
/**
 * @brief   generates the alphabet of all printable (non-whitespace) characters
 * @return  a `std::string` consisting of all printable (non-whitespace) characters
 */
std::string RandAnything<std::string>::alphabet_printable()const{
    return alphabet_alphaLowerCase() + alphabet_alphaUpperCase() + alphabet_numeric() + alphabet_punctuation();
}
/**
 * @brief   generates the alphabet of all hexadecimal digits [0,f]
 * @return  a `std::string` consisting of all hexadecimal digits [0,f]
 */
std::string RandAnything<std::string>::alphabet_hexadecimal()const{
    std::string result = alphabet_numeric();
    for(char ch='a'; ch <= 'f'; ++ch)
        result += ch;
    return result;
}

#endif