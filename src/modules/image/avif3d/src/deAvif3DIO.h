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

#ifndef _DEAVIF3DIO_H_
#define _DEAVIF3DIO_H_

#include <avif/avif.h>
#include <dragengine/common/collection/decTList.h>

class decBaseFileReader;


/**
 * AVIF-3D IO handler.
 */
class deAvif3DIO{
private:
	decBaseFileReader &pReader;
	decTList<uint8_t> pBuffer;
	avifIO pIO;
	
	static avifResult ReadCallback(struct avifIO *io, uint32_t readFlags,
		uint64_t offset, size_t size, avifROData *out);
	static void DestroyCallback(struct avifIO *io);
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create AVIF-3D IO. */
	explicit deAvif3DIO(decBaseFileReader &reader);
	
	/** Clean up AVIF-3D IO. */
	~deAvif3DIO();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** AVIF IO structure for use with decoder. */
	inline avifIO &GetIO(){ return pIO; }
	inline const avifIO &GetIO() const{ return pIO; }
	/*@}*/
};

#endif
