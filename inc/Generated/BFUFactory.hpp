/*
 *
 *  THIS FILE IS AUTOGENERATED.
 *
 *  PROVIDES createBFU(std::string) FUNCTION.
 *  THIS FUNCTION RETURNS A BFU OF THE GIVEN STRING NAME.
 *
 */

// #include <memory>
// #include <cassert>
// #include <cstdarg>

// #include "../IOUnit.hpp"
// #include "FunctionalUnit.hpp"

// std::unique_ptr<FunctionalUnit> createBFU(std::string name...)
// {
//     va_list args;
//     va_start(args, name);
//     if (name == "io")
//     {
//         // IOUnit can always get to the registerfile just fine
//         va_arg(args, int); // clear out this arg. bool is promoted to int on VAArg
//         return std::make_unique<IOUnit>(true, va_arg(args, unsigned));
//     }
//     assert(false && "unknown bfu name during construction");
// }
