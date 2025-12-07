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

#ifndef _DEWEBPIMAGEINFO_H_
#define _DEWEBPIMAGEINFO_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/systems/modules/image/deBaseImageInfo.h>

class deWebpModule;
class decBaseFileReader;


/**
 * Image information.
 */
class deWebpImageInfo : public deBaseImageInfo{
private:
	decString pFilename;
	decMemoryFile::Ref pData;
	int pWidth;
	int pHeight;
	bool pHasAlpha;
	bool pIsGrayscale;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image information. */
	deWebpImageInfo(decBaseFileReader &reader);
	
	/** Clean up image information. */
	virtual ~deWebpImageInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Filename. */
	inline const decString &GetFilename() const{return pFilename;}
	
	/** Data. */
	inline const decMemoryFile &GetData() const{return pData;}
	
	/** Has alpha. */
	inline bool GetHasAlpha() const{return pHasAlpha;}
	
	/** Use grayscale. */
	inline bool GetIsGrayscale() const{return pIsGrayscale;}
	
	/** Image width in pixels. */
	virtual int GetWidth();
	
	/** Image height in pixels. */
	virtual int GetHeight();
	
	/** Image depth in pixels. */
	virtual int GetDepth();
	
	/** Image component count. */
	virtual int GetComponentCount();
	
	/** Image bit count. */
	virtual int GetBitCount();
	
	/** Assert status code. .*/
	void Assert(VP8StatusCode statusCode) const;
	/*@}*/
};

#endif
