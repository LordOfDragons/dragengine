/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLRFONTSIZE_H_
#define _DEOGLRFONTSIZE_H_

#include "deoglRFontGlyphs.h"

#include <dragengine/deObject.h>


/**
 * Render font size.
 */
class deoglRFontSize : public deObject{
public:
	typedef deTObjectReference<deoglRFontSize> Ref;
	
	
private:
	deoglRFont &pFont;
	const deFontSize &pSize;
	deoglRFontGlyphs pGlyphs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render font size. */
	deoglRFontSize(deoglRenderThread &renderThread, deoglRFont &font, const deFontSize &size);
	
protected:
	/** Clean up render font. */
	~deoglRFontSize() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Font. */
	inline deoglRFont &GetFont() const{return pFont;}
	
	/** Font size. */
	inline const deFontSize &GetFontSize() const{return pSize;}
	
	/** Glyphs. */
	inline deoglRFontGlyphs &GetGlyphs(){return pGlyphs;}
	inline const deoglRFontGlyphs &GetGlyphs() const{return pGlyphs;}
	
	/** Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	/*@}*/
};

#endif
