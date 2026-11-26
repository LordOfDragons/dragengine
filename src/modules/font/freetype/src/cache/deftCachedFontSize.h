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

#ifndef _DEFTCACHEDFONTSIZE_H_
#define _DEFTCACHEDFONTSIZE_H_

#include "../layout/deftLayout.h"

#include <dragengine/threading/deThreadSafeObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>

class deftModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Disk cache of font size glyph layout and atlas image.
 */
class deftCachedFontSize : public deThreadSafeObject{
public:
	using Ref = deTThreadSafeObjectReference<deftCachedFontSize>;
	
	
private:
	deftModule &pModule;
	deFont::Ref pFont;
	deFontSize::Ref pFontSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cached font size. */
	deftCachedFontSize(deftModule &module, const deFont::Ref &font, const deFontSize::Ref &fontSize);
	
protected:
	/** Clean up cached font size. */
	~deftCachedFontSize() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Font. */
	inline const deFont::Ref &GetFont() const{ return pFont; }

	/** Font size. */
	inline const deFontSize::Ref &GetFontSize() const{ return pFontSize; }
	
	
	/** Read from cache. Returns true if successful, false otherwise. */
	bool ReadFromCache(decBaseFileReader &reader);
	
	/** Write to cache. */
	void WriteToCache(decBaseFileWriter &writer) const;
	/*@}*/
};

#endif
