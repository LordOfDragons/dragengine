/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DETYPETRAITS_H_
#define _DETYPETRAITS_H_

#include <type_traits>

/**
 * \brief Type constraint "T is constructibler".
 * 
 * C++20 requires to obey the Rule of 5 meaning for a type to considered copyable
 * the type class requires these five functions to be defined:
 * - T::T(const T&) (copy constructor)
 * - T::T(T&&) (move constructor)
 * - T &T::operator=(const T&) (copy assignment)
 * - T &T::operator=(T&&) (move assignment)
 * - T::~T() (destructor)
 * 
 * However, in many cases only the copy constructor is required to be defined.
 * This concept defines a "loose" copyable constraint only requiring the copy
 * constructor to be defined.
 */

// Helper to detect whether a type is complete (sizeof(T) is well-formed).
/*
template<typename T, typename = void>
struct de_is_complete : std::false_type {};

template<typename T>
struct de_is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

template<typename T>
concept de_copy_constructible = de_is_complete<T>::value && std::is_copy_constructible_v<T>;
*/

template<typename T>
concept de_copy_constructible = std::is_copy_constructible_v<T>;

#endif
