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
#ifndef _METERRAINMASKIMAGE_H_
#define _METERRAINMASKIMAGE_H_

// includes
#include "dragengine/resources/image/deImage.h"



/**
 * @brief Mask Image Wrapper.
 *
 * Wraps a mask image delivering the mask value without having to worry about
 * the underlaying bit count of the height image. The image is not held so you
 * have to hold the image yourself. The data is only wrapped in a shallow way
 * without any checks done so do the checks yourself.
 */
class meTerrainMaskImage{
private:
	int pWidth;
	int pHeight;
	sGrayscale8 *pData8;
	sGrayscale16 *pData16;
	sGrayscale32 *pData32;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new terrain mask image wrapper. */
	meTerrainMaskImage( deImage *image );
	/** Cleans up the terrain mask image wrapper. */
	~meTerrainMaskImage();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the width. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height. */
	inline int GetHeight() const{ return pHeight; }
	/**
	 * Retrieves the mask value at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	unsigned char GetMaskValueAt( int x, int y ) const;
	/**
	 * Retrieves the mask value at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	unsigned char GetMaskValueAt( int index ) const;
	/**
	 * Sets the mask value at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	void SetMaskValueAt( int x, int y, unsigned char value );
	/**
	 * Sets the mask value at the given position. No checking is done on
	 * the parameters so make sure they are inside the boundaries.
	 */
	void SetMaskValueAt( int index, unsigned char value );
	/*@}*/
};

// end of include only once
#endif
