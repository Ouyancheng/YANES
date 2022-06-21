#pragma once

#define TUPLE_FST(...) TUPLE_FST_S(__VA_ARGS__)
#define TUPLE_SND(...) TUPLE_SND_S(__VA_ARGS__)
#define TUPLE_FST_S(A, B) A
#define TUPLE_SND_S(A, B) B

#define TUPLE_APPLY(FUNC, TUP) TUPLE_APPLY_S(FUNC TUP)
#define TUPLE_APPLY_S(X) X
#define MACRO_CONCAT(...) MACRO_CONCAT_S(__VA_ARGS__)
#define MACRO_CONCAT_S(A, B) A##B
#define MACRO_EXPAND_TO_STR(...) MACRO_EXPAND_TO_STR_S(__VA_ARGS__)
#define MACRO_EXPAND_TO_STR_S(EXPANDED) #EXPANDED

/**
 * useful: 
 * https://gcc.gnu.org/onlinedocs/cpp/Macro-Arguments.html 
 * https://gcc.gnu.org/onlinedocs/cpp/Macro-Pitfalls.html#Macro-Pitfalls
 * https://gcc.gnu.org/onlinedocs/cpp/Argument-Prescan.html#Argument-Prescan
 * https://stackoverflow.com/questions/12824703/array-format-for-define-c-preprocessor
 * https://stackoverflow.com/questions/11693219/macro-expansion-and-stringification-how-to-get-the-macro-name-not-its-value-s
 * https://stackoverflow.com/questions/48465770/c-preprocessor-macro-concatenate-arguments-after-expansion
 * https://stackoverflow.com/questions/28526930/expanding-recombining-macro-tuple
 */
