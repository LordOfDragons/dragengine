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

#ifndef _DEAVIF3DIMAGEINFO_H_
#define _DEAVIF3DIMAGEINFO_H_

#include <dragengine/systems/modules/image/deBaseImageInfo.h>


/**
 * Image information.
 */
class deAvif3DImageInfo : public deBaseImageInfo{
private:
	decString pFilename;
	int pWidth, pHeight, pDepth, pComponentCount, pBitCount;
	bool pHasAlpha, pIsGrayscale;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image information. */
	explicit deAvif3DImageInfo(const decString &filename);
	
	/** Clean up image information. */
	~deAvif3DImageInfo() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Has alpha. */
	inline bool GetHasAlpha() const{ return pHasAlpha; }
	
	/** Use grayscale. */
	inline bool GetIsGrayscale() const{ return pIsGrayscale; }
	
	/** Image width in pixels. */
	int GetWidth() override;
	
	/** Image height in pixels. */
	int GetHeight() override;
	
	/** Image depth in pixels. */
	int GetDepth() override;
	
	/** Image component count. */
	int GetComponentCount() override;
	
	/** Image bit count. */
	int GetBitCount() override;
	
	
	/** \name Properties */
	/*@{*/
	/** Set width. */
	inline void SetWidth(int width){ pWidth = width; }
	
	/** Set height. */
	inline void SetHeight(int height){ pHeight = height; }
	
	/** Set depth. */
	inline void SetDepth(int depth){ pDepth = depth; }
	
	/** Set component count. */
	inline void SetComponentCount(int count){ pComponentCount = count; }
	
	/** Set bit count. */
	inline void SetBitCount(int count){ pBitCount = count; }
	
	/** Set has alpha. */
	inline void SetHasAlpha(bool hasAlpha){ pHasAlpha = hasAlpha; }
	
	/** Set is grayscale. */
	inline void SetIsGrayscale(bool isGrayscale){ pIsGrayscale = isGrayscale; }
	/*@}*/
};

#endif
