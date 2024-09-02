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

#ifndef _DEBASEIMAGEINFOS_H_
#define _DEBASEIMAGEINFOS_H_

#include "../deBaseModule.h"



/**
 * \brief Image Module Image Information.
 *
 * Provides information for an image to loading afterwards. Can be used
 * by the image module to store additional data structures needed for loading
 * in a subclasses object.
 */
class DE_DLL_EXPORT deBaseImageInfo{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new image info object. */
	deBaseImageInfo();
	
	/** \brief Clean up image info object. */
	virtual ~deBaseImageInfo();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Retrieves the width in pixels. */
	virtual int GetWidth() = 0;
	
	/** \brief Retrieves the height in pixels. */
	virtual int GetHeight() = 0;
	
	/** \brief Retrieves the depth in pixels. */
	virtual int GetDepth() = 0;
	
	/** \brief Retrieves the number of components. */
	virtual int GetComponentCount() = 0;
	
	/** \brief Retrieves the bit count of each component. */
	virtual int GetBitCount() = 0;
	/*@}*/
};

#endif
