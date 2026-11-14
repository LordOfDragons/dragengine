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

#ifndef _DEFTRENDERERSDF_H_
#define _DEFTRENDERERSDF_H_

#include <ft2build.h>
#include FT_FREETYPE_H

class deftLayout;
class deftModule;
class deFontSize;
class deImage;


/**
 * Render glyph map.
 */
class deftRendererSdf{
private:
	deftModule &pModule;
	FT_Face pFace;
	FT_Int32 pLoadFlags;
	double pUnitScale, pSdfMinVal, pSdfMaxVal, pPixelRange;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deftRendererSdf(deftModule &module, FT_Face face);
	
	/** Clean up renderer. */
	~deftRendererSdf();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Load flags for FreeType glyph loading. */
	inline FT_Int32 GetLoadFlags() const{ return pLoadFlags; }
	
	/** Set load flags for FreeType glyph loading. */
	void SetLoadFlags(FT_Int32 loadFlags);
	
	/** Get the unit scale for SDF rendering. */
	inline double GetUnitScale() const{ return pUnitScale; }
	
	/** Set the unit scale for SDF rendering. */
	void SetUnitScale(double unitScale);
	
	/** Get the SDF distance range. */
	inline double GetSdfMinVal() const{ return pSdfMinVal; }
	inline double GetSdfMaxVal() const{ return pSdfMaxVal; }
	
	/** Set the SDF distance range. */
	void SetSdfDistanceRange(double minVal, double maxVal);
	
	/** Get the pixel range for SDF rendering. */
	inline double GetPixelRange() const{ return pPixelRange; }

	/** Set the pixel range for SDF rendering. */
	void SetPixelRange(double pixelRange);
	
	
	/** Render glyph map. */
	void Render(const deftLayout &layout, deFontSize &fontSize);
	/*@}*/
	
	
private:
	void pRenderSdf(const deftLayout &layout, deImage &image);
};

#endif
