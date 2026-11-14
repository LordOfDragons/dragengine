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

#ifndef _DEFTMODULE_H_
#define _DEFTMODULE_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include "cache/deftCaches.h"

#include <dragengine/systems/modules/font/deBaseFontModule.h>

class deFontGlyph;


/**
 * FreeType Font module.
 */
class deftModule : public deBaseFontModule {
private:
	FT_Library pFTLib;
	deftCaches::Ref pCaches;
	bool pCreateSdfFont;
	int pMaxImageSize, pLineHeightLimit, pGlyphSpacing, pSdfLogic;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deftModule(deLoadableModule &loadableModule);
	
	/** Clean up module. */
	~deftModule() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Freetype library. */
	inline FT_Library GetFTLibrary() const{ return pFTLib; }
	
	/** Caches. */
	inline const deftCaches::Ref &GetCaches() const{ return pCaches; }
	
	/** Assert Freetype call. */
	void AssertFT(FT_Error error, const char *function);
	
	/** Create SDF fonts. */
	inline bool GetCreateSdfFont() const { return pCreateSdfFont; }
	
	/** Set to create SDF fonts. */
	void SetCreateSdfFont(bool createSdf);
	
	/** Maximum image size. */
	inline int GetMaxImageSize() const { return pMaxImageSize; }
	
	/** Set maximum image size. */
	void SetMaxImageSize(int maxImageSize);
	
	/** Line height limit. */
	inline int GetLineHeightLimit() const { return pLineHeightLimit; }
	
	/** Set line height limit. */
	void SetLineHeightLimit(int lineHeightLimit);
	
	/** Glyph spacing. */
	inline int GetGlyphSpacing() const { return pGlyphSpacing; }

	/** Set glyph spacing. */
	void SetGlyphSpacing(int glyphSpacing);
	
	/** SDF logic. */
	inline int GetSdfLogic() const { return pSdfLogic; }
	
	/** Set SDF logic. */
	void SetSdfLogic(int sdfLogic);
	
	
	/** Load font. */
	void LoadFont(decBaseFileReader &reader, deFont &font) override;
	
	/**
	 * \brief Load font size.
	 * 
	 * If loading font size is not fast a parallel load task has to be assigned. The load task
	 * has to update the font size instance until loading finished. Users must not touch the
	 * font size until the loader task is finished.
	 * 
	 * Module does creates a new deFontSize instance if the requested font size can be loaded
	 * successfully. If the requested font size is too large the module can create a deFontSize
	 * of smaller line height. In general font modules use a line height limit to avoid creating
	 * huge textures or font render engines not supporting large sizes. If a deFontSize with
	 * the largest supported size exists already the module has to return this one.
	 * 
	 * Default implementation throws deeInvalidAction exception.
	 */
	deFontSize::Ref LoadFontSize(const decBaseFileReader::Ref &reader,
		const deFont::Ref &font, int lineHeight) override;
	
	/** Save font. */
	void SaveFont(decBaseFileWriter &writer, const deFont &font) override;
	/*@}*/
};

#endif
