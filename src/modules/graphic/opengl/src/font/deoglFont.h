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

#ifndef _DEOGLFONT_H_
#define _DEOGLFONT_H_

#include "deoglRFont.h"
#include "../deoglBasics.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicFont.h>

class deFont;
class deGraphicOpenGl;
class deoglRFontSize;


/**
 * Font peer.
 */
class deoglFont : public deBaseGraphicFont{
private:
	deFont &pFont;
	const deoglRFont::Ref pRFont;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create font. */
	deoglFont(deGraphicOpenGl &ogl, deFont &font);
	
	/** Clean up font. */
	~deoglFont() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Font resource. */
	inline deFont &GetFont() const{return pFont;}
	
	/** Render font. */
	inline const deoglRFont::Ref &GetRFont() const{return pRFont;}
	
	/**
	 * Get font size if present in font resource. If not present returns nullptr. Otherwise
	 * ensures deoglRFontSize is present, prepared and ready to be used.
	 * 
	 * \note Mutex protected access to sizes.
	 */
	deoglRFontSize *GetFontSizeFor(int lineHeight);
	/*@}*/
	
	
	/** \name Notifications */
	/*@{*/
	/** Update font. */
	void Update(float elapsed) override;
	
	/** \brief Font size added. */
	void FontSizeAdded(int lineHeight, deFontSize *size) override;
	/*@}*/
};

#endif
