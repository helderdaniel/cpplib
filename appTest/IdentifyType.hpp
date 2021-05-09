//
// Created by hdaniel on 09/05/2021.
//

#ifndef MOOSHAKTOOLS_IDENTIFYTYPE_HPP
#define MOOSHAKTOOLS_IDENTIFYTYPE_HPP

/**
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#56766138
 * https://godbolt.org/z/8x88so
 *
 * include as:
 *              #include <appTest/IdentifyType.hpp>
 *
 * use it as:
 *              auto var = [](auto n) { return n<0; }
 *              std::cout << "type: " << type_name<decltype(var)>() << '\n';
 */

#include <string_view>

template <typename T>
constexpr auto type_name() noexcept {
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto type_name() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void) noexcept";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}


#endif //MOOSHAKTOOLS_IDENTIFYTYPE_HPP
