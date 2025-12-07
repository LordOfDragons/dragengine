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

#ifndef _DEOGLRFONT_H_
#define _DEOGLRFONT_H_

#include "deoglRFontSize.h"
#include "deoglRFontGlyphs.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/threading/deMutex.h>


/**
 * Render font.
 */
class deoglRFont : public deObject{
public:
	typedef deTObjectReference<deoglRFont> Ref;
	
	
private:
	const decString pFilename;
	bool pIsColorFont;
	
	deoglRFontGlyphs pGlyphs;
	
	decObjectList pSizes;
	deMutex pMutex;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render font. */
	deoglRFont(deoglRenderThread &renderThread, const deFont &font);
	
protected:
	/** Clean up render font. */
	~deoglRFont() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Font is a colorable font. */
	inline bool GetIsColorFont() const{ return pIsColorFont; }
	
	/** Glyphs. */
	inline deoglRFontGlyphs &GetGlyphs(){ return pGlyphs; }
	inline const deoglRFontGlyphs &GetGlyphs() const{ return pGlyphs; }
	
	
	/** Finalize after asynchronous resource loading. */
	void FinalizeAsyncResLoading();
	
	
	/**
	 * Get font size if present in font resource. If not present returns nullptr. Otherwise
	 * ensures deoglRFontSize is present, prepared and ready to be used.
	 * 
	 * \note Mutex protected access to sizes.
	 */
	deoglRFontSize *GetFontSizeFor(deFont &font, int lineHeight);
	/*@}*/
};

#endif
