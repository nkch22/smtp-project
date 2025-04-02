/**
 * @file PreprocessorUtilities.hpp
 * @brief Preprocessor utility macros for variadic macro expansion.
 *
 * @details
 * This header provides helper macros for token concatenation, identity, and counting variadic
 * arguments. The C/C++ preprocessor does not directly support operations like counting the number
 * of arguments in a variadic macro or concatenating tokens. These utilities enable us to build
 * higher-level macros (e.g. FOR_EACH) that can iterate over an arbitrary number of arguments.
 *
 * The defined macros are used in our JSON serialization macros to automatically expand an action
 * (such as adding or retrieving a field) across multiple fields in a customizable and modular
 * fashion.
 */

#pragma once

/**
 * @brief Identity macro.
 *
 * @details
 * This macro simply returns the argument passed to it. It is used to force or delay macro
 * expansion in certain contexts. For example, it helps ensure that the result of a token
 * concatenation is properly re-interpreted by the preprocessor.
 *
 * @param x The input token or expression.
 * @return The same token/expression x.
 */
#define PP_IDENTITY(x) x

/**
 * @brief Token concatenation macro.
 *
 * @details
 * The C preprocessor provides the '##' operator for token concatenation. However, when
 * concatenating tokens that may themselves be macros, we need an extra level of indirection.
 * PP_CONCAT first calls PP_CONCAT_IMPL, which then uses the '##' operator to join the tokens.
 *
 * @param a The first token.
 * @param b The second token.
 * @return The concatenation of token a and token b.
 */
#define PP_CONCAT(a, b) PP_CONCAT_IMPL(a, b)
#define PP_CONCAT_IMPL(a, b) a##b

/**
 * @brief Counts the number of variadic arguments.
 *
 * @details
 * These macros determine how many arguments were passed to a variadic macro.
 * Built-in support for variadic argument counting is not available in the C++ preprocessor,
 * so we simulate it by appending a known sequence of numbers (via PP_RSEQ_N) and then selecting
 * the appropriate token (using PP_ARG_N).
 *
 * @param ... The variadic arguments.
 * @return The total number of arguments passed.
 */
#define PP_NARG(...) PP_NARG_IMPL(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_IMPL(...) PP_ARG_N(__VA_ARGS__)

/**
 * @brief Selects the N-th argument.
 *
 * @details
 * The PP_ARG_N macro is used as part of the argument-counting mechanism. It accepts up to 30
 * arguments and returns the 31st parameter, which corresponds to the number of arguments passed.
 *
 * @return The count N.
 */
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22,  \
				 _23, _24, _25, _26, _27, _28, _29, _30, N, ...)                                                       \
	N

/**
 * @brief Reverse sequence of numbers used for counting arguments.
 *
 * @details
 * This macro provides a sequence of numbers in descending order from 30 to 0. It is used to
 * "pad" the variadic argument list so that PP_ARG_N can pick out the correct argument count.
 *
 * @return A sequence of numbers: 30, 29, 28, ..., 1, 0.
 */
#define PP_RSEQ_N()                                                                                                    \
	30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
