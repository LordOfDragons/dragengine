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
#ifndef _DEDSSTNIMAGE_H_
#define _DEDSSTNIMAGE_H_

// includes
#include "dedsStyledTextNode.h"

// predefinitions
class deImage;



/**
 * @brief Styled Text Node Image.
 * Image node in a styled text object storing a single image.
 * The image is an engine image object. The image can be NULL
 * in which case this node is an empty node of 0 size.
 */
class dedsSTNImage : public dedsStyledTextNode{
private:
	deImage *pImage;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new image node. */
	dedsSTNImage();
	/** Creates a new image node with the given image. */
	dedsSTNImage( deImage *image );
	/** Cleans up the image node. */
	virtual ~dedsSTNImage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the image or NULL if not set. */
	inline deImage *GetImage() const{ return pImage; }
	/** Sets the image. */
	void SetImage( deImage *image );
	/*@}*/
};

// end of include only once
#endif
