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

#ifndef _DEFTLAYOUT_H_
#define _DEFTLAYOUT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>

class deFontSize;

/**
 * Glyph layout.
 */
class deftLayout : public deObject{
public:
	using Ref = deTObjectReference<deftLayout>;
	
	
	struct Glyph{
		int unicode = 0;
		decPoint3 position;
		decPoint size, bearing;
		int advance = 0;
	};
	
	using GlyphList = decTList<Glyph>;
	
	
private:
	decPoint3 pImageSize;
	GlyphList pGlyphs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create layout. */
	deftLayout();
	
protected:
	/** Clean up layout. */
	~deftLayout() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Image size. */
	inline decPoint3 GetImageSize() const{ return pImageSize; }
	
	/** Set image size. */
	void SetImageSize(const decPoint3 &size);
	
	/** Glyph list. */
	inline GlyphList &GetGlyphs(){ return pGlyphs; }
	inline const GlyphList &GetGlyphs() const{ return pGlyphs; }
	
	/** Apply layout to font size. */
	void Apply(deFontSize &size) const;
	/*@}*/
};

#endif
