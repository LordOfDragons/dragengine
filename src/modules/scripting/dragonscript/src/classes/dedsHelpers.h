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

#include <cstddef>
#include <utility>

#include <libdscript/libdscript.h>


/** Calculate the required data size to store native data struct. */
template<typename T>
consteval int dedsNativeDataSize(){
	return dsNativeDataSize<T>();
}


/** Perform placement new of native data struct. */
template<typename T, typename... Args>
inline T& dedsNewNativeData(void *buffer, Args&&... args){
	return dsNativeDataNew<T>(buffer, std::forward<Args>(args)...);
}


/** Get native data reference. */
template<typename T>
inline T& dedsGetNativeData(void *buffer){
	return dsNativeDataGet<T>(buffer);
}

#endif
