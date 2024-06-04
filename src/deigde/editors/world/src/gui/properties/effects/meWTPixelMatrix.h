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

#if 0

// include only once
#ifndef _MEWTPIXELMATRIX_H_
#define _MEWTPIXELMATRIX_H_

// includes
#include <dragengine/common/math/decMath.h>



/**
 * @brief Pixel Matrix Template.
 * Pixel Matrix Template for the world editor.
 */
class meWTPixelMatrix{
private:
	char *pName;
	decTexMatrix pPixelMatrix;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new color matrix template object. */
	meWTPixelMatrix( const char *name, const decTexMatrix &matrix );
	/** Cleans up the color matrix template. */
	~meWTPixelMatrix();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name of the template. */
	inline const char *GetName() const{ return ( const char * )pName; }
	/** Retrieves the color matrix. */
	inline const decTexMatrix &GetPixelMatrix() const{ return pPixelMatrix; }
	/** Sets the color matrix. */
	void SetPixelMatrix( const decTexMatrix &matrix );
	/*@}*/
};

// end of include only once
#endif

#endif
