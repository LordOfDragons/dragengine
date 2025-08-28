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

#ifndef _DEFTFONT_H_
#define _DEFTFONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/font/deFontSize.h>

class decBaseFileReader;
class deftModule;
class deFont;
class deImage;


/**
 * FreeType Font module.
 */
class deftFont{
private:
	deftModule &pModule;
	decBaseFileReader &pReader;
	FT_Face pFace;
	FT_StreamRec pStreamRec;
	FT_Open_Args pOpenArgs;
	int pMaxImageWidth, pGlyphSpacing, pLineHeightLimit;
	int pRealFontSize, pBaseLine, pRealLineHeight;
	decPoint pBearingOffset;
	decBoundary pBBox;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create module. */
	deftFont(deftModule &module, decBaseFileReader &reader);
	
	/** Clean up module. */
	~deftFont();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Load font. */
	void LoadFont(deFont &font);
	
	/** Load font size. */
	deFontSize::Ref LoadFontSize(deFont &font, int lineHeight);
	/*@}*/
	
	
private:
	int pCountUsableGlyphs() const;
	void pInitGlyphs(deFont &font);
	int pFindBestSize(const deFont &font, const deFontSize &size);
	void pInitGlyphs(deFontSize &size);
	decPoint3 pCalcLayout(deFontSize &size);
	void pDebugPrintGlyphs(const deFont &font, const deFontSize &size);
	void pRenderGlyphs(const deFontSize &size, deImage &image);
	
	static unsigned long fStreamRead(FT_Stream stream, unsigned long offset,
		unsigned char *buffer, unsigned long count);
	
	static void fStreamClose(FT_Stream);
};

#endif
