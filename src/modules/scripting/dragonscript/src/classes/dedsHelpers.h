/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEDEDSHELPERS_H_
#define _DEDEDSHELPERS_H_

#include <new>       // For std::launder and placement new
#include <cstddef>   // For std::max_align_t
#include <cstdint>   // For uintptr_t


// 2026 Recommended: Align to at least 16 bytes on Windows to prevent SIMD crashes.
template<typename T>
consteval size_t dedsRequiredAlignment(){
	// std::max_align_t is often 16 on modern Windows/Linux to support x64 SIMD.
	return (alignof(T) > alignof(std::max_align_t)) ? alignof(T) : alignof(std::max_align_t);
}


/** Calculate the required data size to store native data struct. */
template<typename T>
inline size_t dedsNativeDataSize(){
	return sizeof(T) + (dedsRequiredAlignment<T>() - 1);
}


/** Perform placement new of native data struct. */
template<typename T>
inline T& dedsNewNativeData(void *buffer){
	const uintptr_t addr = reinterpret_cast<uintptr_t>(buffer);
	const size_t align = dedsRequiredAlignment<T>();
	const uintptr_t alignedAddr = (addr + (align - 1)) & ~(align - 1);
	
	// Construction with () for zero-initialization of primitives
	return *(new (reinterpret_cast<T*>(alignedAddr)) T());
}

/** Get native data reference. */
template<typename T>
inline T& dedsGetNativeData(void *buffer){
	const uintptr_t addr = reinterpret_cast<uintptr_t>(buffer);
	const size_t align = dedsRequiredAlignment<T>();
	const uintptr_t aligned_addr = (addr + (align - 1)) & ~(align - 1);
	
	// std::launder is MANDATORY in 2026 to prevent MSVC from 
	// caching old pointer values in registers during optimization.
	return *std::launder(reinterpret_cast<T*>(aligned_addr));
}

#endif
