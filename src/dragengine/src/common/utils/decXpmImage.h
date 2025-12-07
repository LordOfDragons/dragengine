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

#ifndef _DECXPMIMAGE_H_
#define _DECXPMIMAGE_H_

#include "../../dragengine_export.h"


/**
 * \brief Simple XPM Image container.
 *
 * A simple parser for XPM images linked into the code.
 * It is very basic and stores the image as RGB format.
 */
class DE_DLL_EXPORT decXpmImage{
private:
	int pWidth, pHeight;
	char *pData;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create new xpm image object from the given xmp data source.
	 * \param xpmData Pointer to the xpm data. Usually the pointer to the compiled in xpm data.
	 * \param flip Determines if the image has to be flippep upside down.
	 */
	decXpmImage(const char *xpmData[], bool flip);
	
	/** \brief Clean up xpm image object. */
	~decXpmImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Width of the image. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of the image. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Pointer to the RGB aligned data. */
	inline const char *GetData() const{ return pData; }
	/*@}*/
	
	
	
private:
	int pReadInt(const char **pdata);
	int pReadColor(const char **pdata);
};

#endif
