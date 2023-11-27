/*
 *  Copyright (C) 2023 Callum Gran
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*==========================================================================*

  FILE
    lambda.h

  PROJECT
    hurust generic library

  DESCRIPTION
    This file contains a macro for creating lambda functions.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    25-Nov-23  C.Gran		Created file.

 *==========================================================================*/
#ifndef HURUST_FUNCTIONAL_LAMBDA_H
#define HURUST_FUNCTIONAL_LAMBDA_H

/**
 * \brief     A macro for defining a lambda function.
 * \note      This macro defines a lambda function with the given return type,
 *            function arguments, and function body.
 * \param[in] return_type The return type of the lambda function.
 * \param[in] func_args The arguments of the lambda function.
 * \param[in] func_body The body of the lambda function.
 */
#define lambda(return_type, func_args, func_body) \
    ({ return_type __fn_t__ func_args func_body &__fn_t__; })

/**
 * \brief     A macro for defining an anonymous function.
 * \note      This macro defines an anonymous function with the given return
 *            type, function arguments, and function body.
 * \param[in] return_type The return type of the anonymous function.
 * \param[in] func_args The arguments of the anonymous function.
 * \param[in] func_body The body of the anonymous function.
 * \param[in] ... The arguments to the anonymous function.
 */
#define anon(return_type, func_args, func_body, ...) \
    lambda(return_type, func_args, func_body)(__VA_ARGS__)

#endif // HURUST_FUNCTIONAL_LAMBDA_H