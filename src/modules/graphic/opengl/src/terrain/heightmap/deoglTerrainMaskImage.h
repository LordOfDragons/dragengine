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

#ifndef _DEOGLTERRAINMASKIMAGE_H_
#define _DEOGLTERRAINMASKIMAGE_H_

#include <dragengine/resources/image/deImage.h>


/**
 * Mask Image Wrapper.
 *
 * Wraps a mask image delivering the mask value without having to worry about the underlaying
 * bit count of the mask image. The image is not held so you have to hold the image yourself.
 * The data is only wrapped in a shallow way without any checks done so do the checks yourself.
 * 
 * \warning This class retains image data in the constructor and release it in the destructor.
 */
class deoglTerrainMaskImage{
private:
	deImage &pImage;
	int pWidth;
	int pHeight;
	sGrayscale8 *pData8;
	sGrayscale16 *pData16;
	sGrayscale32 *pData32;
	bool pRetained;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create terrain mask image wrapper. */
	deoglTerrainMaskImage(deImage &image);
	
	/** Clean up terrain mask image wrapper. */
	~deoglTerrainMaskImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Width. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height. */
	inline int GetHeight() const{ return pHeight; }
	
	/**
	 * Mask value at position.
	 * 
	 * No checking is done on the parameters so make sure they are inside the boundaries.
	 */
	float GetMaskValueAt(int x, int y) const;
	
	/**
	 * Mask value at position.
	 * 
	 * No checking is done on the parameters so make sure they are inside the boundaries.
	 */
	float GetMaskValueAt(int index) const;
	/*@}*/
};

#endif
