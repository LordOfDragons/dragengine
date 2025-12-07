/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECBASE64_H_
#define _DECBASE64_H_

#include <stdint.h>

#include "../string/decString.h"
#include "../../dragengine_export.h"


/**
 * \brief Base 64 encoding/decoding.
 * \version 1.23
 */
class DE_DLL_EXPORT decBase64{
private:
	decBase64();
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Encode using Base64. */
	static decString Encode(const decString &in);
	static decString Encode(const uint8_t *in, int inLen);
	static void Encode(const uint8_t *in, int inLen, char *out, int outSize, int &outLen);
	
	/** \brief Safe size of buffer required to hold Base64 encoded. */
	static int EncodeSize(int inLen);
	
	/** \brief Decode using Base64. */
	static void Decode(const decString &in, uint8_t *out, int outSize, int &outLen);
	static void Decode(const char *in, int inLen, uint8_t *out, int outSize, int &outLen);
	
	/** \brief Safe size of buffer required to hold Base64 decoded. */
	static int DecodeSize(int inLen);
	/*@}*/
};

#endif
