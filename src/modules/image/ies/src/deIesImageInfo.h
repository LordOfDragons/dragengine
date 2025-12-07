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

#ifndef _DEIESIMAGEINFO_H_
#define _DEIESIMAGEINFO_H_

#include <dragengine/common/string/decStringList.h>
#include <dragengine/systems/modules/image/deBaseImageInfo.h>

class decBaseFileReader;


/**
 * \brief Image information class.
 */
class deIesImageInfo : public deBaseImageInfo{
protected:
	int pWidth;
	int pHeight;
	int pDepth;
	decStringList pLines;
	int pCurLine;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create image information. */
	deIesImageInfo();
	
	/** \brief Clean up image information. */
	virtual ~deIesImageInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the width in pixels. */
	virtual int GetWidth();
	
	/** \brief Retrieves the height in pixels. */
	virtual int GetHeight();
	
	/** \brief Retrieves the depth in pixels. */
	virtual int GetDepth();
	
	/** \brief Retrieves the number of components. */
	virtual int GetComponentCount();
	
	/** \brief Retrieves the bit count of each component. */
	virtual int GetBitCount();
	
	/** \brief Load header. */
	virtual bool LoadHeader(decBaseFileReader &reader) = 0;
	
	/** \brief Load file. */
	virtual void LoadFile(unsigned short *pixels) = 0;
	/*@}*/
	
	
	
protected:
	void pReadLines(decBaseFileReader &reader);
};

#endif
