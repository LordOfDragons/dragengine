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

#ifndef _DECGLOBALFUNCTIONSSAFE_H_
#define _DECGLOBALFUNCTIONSSAFE_H_

#include <string.h>
#include <stdint.h>
#include <type_traits>
#include <utility>


/** \brief Global hash functions used for example with decTDictionary. */
inline unsigned int DEHash(int key){
	return static_cast<unsigned int>(key);
}

inline unsigned int DEHash(unsigned int key){
	return key;
}

inline unsigned int DEHash(const void *key){
	return key ? static_cast<unsigned int>(reinterpret_cast<uintptr_t>(key)) : 0;
}

template<typename T, typename = std::enable_if_t<
	std::is_convertible<
		decltype(std::declval<const T&>().Hash()),
		unsigned int
	>::value
>>
inline unsigned int DEHash(const T &key){
	return key.Hash();
}


/** \brief Global compare functions used with ordered lists. */
inline int DECompare(int a, int b){
	return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

inline int DECompare(float a, float b){
	return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

inline int DECompare(double a, double b){
	return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

inline int DECompare(const void *a, const void *b){
	return (a < b) ? -1 : ((a > b) ? 1 : 0);
}

template<typename T, typename = std::enable_if_t<
	std::is_convertible<
		decltype(std::declval<T&>().Compare(std::declval<const T&>())),
		int
	>::value
>>
inline int DECompare(const T &a, const T &b){
	return a.Compare(b);
}


#endif
