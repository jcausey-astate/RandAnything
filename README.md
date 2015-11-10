RandAnything
============
Because _pseudo_-**random** should be **easy**.

This is a single-file (_<tt>RandAnything.h</tt>_) library that defines an easy-to-use pseudo-random number generator `RandAnything`.  `RandAnything` can generate random values within a range for almost any numerical type, and even strings!


## Using the Header File in your Project
All you need to do to use `RandAnything` is to place a copy of the header file _<tt>RandAnything.h</tt>_ into your project (or a system include folder like <tt>/usr/local/include/</tt>) and then `#include` it in your program.  

**Easy.**

## Generating Random Numeric Values
Now, you are two steps away from random value bliss. First, declare an instance of the `RandAnything` generator, using a template argument corresponding to the kind of value you want to generate.  For example, if you want to generate integer values to simulate a 6-sided die roll, you do this:
```cpp
RandAnything<int> die_roller; // Generator for integer values
```
Second, you just call the generator object as if it were a function, passing the lower- and upper- bounds for the value you want to generate:
```cpp
int d6 = die_roller(1, 6);   // Simulate rolling a 6-sided die.
```
That's it!  The 1 represents the _low_ end of the range of possible values, and the 6 represents the _high_ end of the range.  A uniform random number in the range [1,6] will be returned.

`RandAnything` will allow you to generate any kind of numeric value you want by following those two steps.

**Range Note:** The range of values generated for integer-based values is a _closed range_ [low, high], but floating-point-based values are produced in an _open range_ [low, high) (the high value is never actually produced, but values can get vanishingly close, according to the precision available in the type itself).

## Generating `std::string` Values
`RandAnything` can also generate randomized strings of type `std::string`!  The process is nearly the same, but there are a few more options.

For one thing, you can choose the _alphabet_ (set of characters) from which the string is chosen.  By default it will consist of all _non-whitespace, printable_ characters (that is, characters that aren't whitespace and will result in a visible character on the screen).  But you can customize the alphabet easily (more on that later).

Also, you can choose to _either_ generate strings with a _variable length_ (whose length is in a range specified by [low, high] similar to the numeric ranges shown above), or you can generate strings with a fixed length.

Example Code:
```cpp
// Set up the generator for strings:
RandAnything<std::string> string_gen;
// Generate a string with a length between 4 and 12 characters:
std::string random_str_1 = string_gen(4,12);

// Generate a string whose length is exactly 8 characters:
std::string random_str_2 = string_gen(8);

// Generate a string of exactly 8 characters while limiting the
// alphabet to "01" (producing something that looks like a 
// binary number):
std::string random_str_3 = string_gen(8, "01");

// Generate a string with between 3 and 9 characters from the 
// alphabet "abc123"
std::string random_str_4 = string_gen(3, 9, "abc123");
```

### Bonus: Easy Alphabets
The `std::string` version of `RandAnything` knows about several special alphabets that will come in handy as well.  The following methods are available:
* `alphabet_alphaLowerCase()` - lowercase letters
* `alphabet_alphaUpperCase()` - uppercase letters
* `alphabet_alphaAllCase()` - lowercase and uppercase letters
* `alphabet_numeric()` - numeric digits 0 through 9
* `alphabet_alphaNumeric()` - lowercase and uppercase letters and numeric digits
* `alphabet_punctuation()` - punctuation characters (non-whitespace printable characters that aren't letters or numbers)
* `alphabet_printable()` - all printable non-whitespace characters
* `alphabet_hexadecimal()` - characters that are valid for hexadecimal digits (`0` through `9` and `a` through `f`)

Examples:
```cpp
// Set up the generator for strings:
RandAnything<std::string> string_gen;

// Generate an alphanumeric password:
std::string pass = string_gen(32, string_gen.alphabet_alphaNumeric());

// Generate a string of lowercase letters between 4 and 16 characters long:
std::string lc   = string_gen(4, 16, string_gen.alphabet_alphaLowerCase());
```

## Fiddly Details
`RandAnything` is designed to give a good trade-off between quality pseudo-random values and performance when generating lots of values. Although some performance was sacrificed for ease-of-use, but it should still be plenty fast enough for most applications.

**Seeding**

When you declare the instance of the `RandAnything` object, it will perform an initial setup operation involving obtaining a source of randomness using a source of _true random entropy_ if such a source is available.  (The mechanism used is the `std::random_device`: see http://en.cppreference.com/w/cpp/numeric/random/random_device).  If a source of nondeterministic entropy is not available, then you end up with a deterministic seeding function.

If you wish, you can supply an explicit seed value when you construct the `RandAnything` object.  If you use an explicit seed, the generator will **always** generate the same sequence of random numbers!  Normally you do not want to do this, but this behavior might be useful in certain testing situations to allow you to "replay" a specific set of outputs.  Here is an example of how you can specify a seed:
```cpp
// Create an integer value generator with an explicit seed of 33:
RandAnything test_gen{33};
```

**Generation**

Random number generation depends on a Mersenne Twister algorithm (specifically `std::mt19937`: see http://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine).  This produces _pseudo-random_ values based on a (in most cases) true-random seed.  The result is that the part of the process that takes significant time (the true-random seeding) occurs only at instantiation, and on every call a comparatively _fast_ pseudo-random generation occurs.

## License and Copyright

The MIT License (MIT)  (http://opensource.org/licenses/MIT)

Copyright (c) 2015 Jason L Causey, Arkansas State University

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 

