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

#ifndef _DEFTLAYOUTER_H_
#define _DEFTLAYOUTER_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include "deftLayout.h"

class deftFont;
class deftModule;
class deFont;
class deFontSize;


/**
 * Calculate glyph layout.
 */
class deftLayouter{
protected:
	deftModule &pModule;
	const deFont &pFont;
	const deFontSize &pFontSize;
	FT_Face pFace;
	
	FT_Int32 pLoadFlags;
	int pMaxImageWidth, pGlyphSpacing, pHintingSafetyMargin;
	
	decBoundary pBoundaryBox;
	int pLineHeight, pBaseLine, pLineHeightMargin;
	decPoint3 pImageSize;
	decPoint pBearingOffset;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create layouter. */
	deftLayouter(deftModule &module, const deFont &font, const deFontSize &fontSize, FT_Face face);
	
	/** Clean up layouter. */
	virtual ~deftLayouter();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Load flags for FreeType glyph loading. */
	inline FT_Int32 GetLoadFlags() const{ return pLoadFlags; }
	
	/** Set load flags for FreeType glyph loading. */
	void SetLoadFlags(FT_Int32 loadFlags);
	
	/** Maximum image width. */
	inline int GetMaxImageWidth() const{ return pMaxImageWidth; }
	
	/** Set maximum image width. */
	void SetMaxImageWidth(int maxImageWidth);
	
	/** Glyph spacing. */
	inline int GetGlyphSpacing() const{ return pGlyphSpacing; }
	
	/** Set glyph spacing. */
	void SetGlyphSpacing(int glyphSpacing);
	
	/** Hinting safety margin. */
	inline int GetHintingSafetyMargin() const{ return pHintingSafetyMargin; }
	
	/** Set hinting safety margin. */
	void SetHintingSafetyMargin(int hintingSafetyMargin);
	
	
	/** Calculate glyph layout. If layout failed nullptr is returned. */
	virtual deftLayout::Ref Layout();
	
	
	/** Image size. */
	inline const decPoint3 &GetImageSize() const{ return pImageSize; }
	
	/** Height of a line of glyphs. */
	inline int GetLineHeight() const{ return pLineHeight; }
	/*@}*/
	
	
protected:
	/**
	 * Find font size resulting in line height closest to the desired line height.
	 * 
	 * Sets these member variables:
	 * - pLineHeight
	 * - pLineHeightMargin
	 * - pBoundaryBox
	 */
	virtual void pFindBestFontSizeForLineHeight();
	
	/**
	 * Calculate bearing offset and base line.
	 * 
	 * Sets these member variables:
	 * - pBearingOffset
	 * - pBaseLine
	 */
	virtual void pCalcBearingOffsetAndBaseLine();
	
	/**
	 * Debug print layout parameters after calculation step.
	 */
	virtual void pDebugPrintLayout() const;
	
	/**
	 * Create layout using the found parameters. Calls pInitGlyphs() and pLayoutGlyphs().
	 * Returns nullptr if pLayoutGlyphs() returns false. Otherwise returns the created layout.
	 */
	virtual deftLayout::Ref pCreateLayout();
	
	/**
	 * Initialize glyphs in the given layout.
	 * 
	 * Requires:
	 * - pLineHeight
	 * - pBearingOffset
	 * - pBaseLine
	 */
	virtual void pInitGlyphs(deftLayout &layout);
	
	/**
	 * Layout glyphs in the given layout. Returns true if layout succeeded.
	 */
	virtual bool pLayoutGlyphs(deftLayout &layout);
};

#endif
