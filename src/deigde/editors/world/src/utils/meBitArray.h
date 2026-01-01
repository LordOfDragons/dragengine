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

// include only once
#ifndef _MEBITARRAY_H_
#define _MEBITARRAY_H_

// includes
#include <dragengine/common/math/decMath.h>



/**
 * @brief Bit Array.
 *
 * Stores a rectangular grid of boolean values in form of a bit array.
 */
class meBitArray{
private:
	int pColons;
	int pRows;
	unsigned char *pBytes;
	int pByteCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new bit array. */
	meBitArray(int colons, int rows);
	/** Cleans up the bit array. */
	~meBitArray();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of colons. */
	inline int GetColons() const{ return pColons; }
	/** Retrieves the number of rows. */
	inline int GetRows() const{ return pRows; }
	/** Retrieves the boolean value at the given location. */
	bool GetValueAt(int x, int y) const;
	/** Sets the boolean value at the given location. */
	void SetValueAt(int x, int y, bool value);
	/** Copies the values to another bit array. */
	void CopyTo(meBitArray &bitArray) const;
	/** Clears all values. */
	void ClearTo(bool value);
	/*@}*/
};

// end of include only once
#endif
