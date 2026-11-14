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

#ifndef _DEFTSDFLAYOUTER_H_
#define _DEFTSDFLAYOUTER_H_

#include "deftLayouter.h"


/**
 * \brief Glyph layout calculation for SDF font images.
 *
 * SDF images do not use additional layers. Glyphs not fitting into the image are skipped.
 */
class deftLayouterSdf : public deftLayouter{
protected:
	int pSdfSize;
	double pSdfPixelRange;
	
	int pBaseLineMargin;
	double pUnitScale, pSdfMinVal, pSdfMaxVal;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create layouter. */
	deftLayouterSdf(deftModule &module, const deFont &font, const deFontSize &fontSize, FT_Face face);
	
	/** Clean up layouter. */
	~deftLayouterSdf() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Sdf size in pixels. */
	inline int GetSdfSize() const{ return pSdfSize; }
	
	/** Set Sdf size in pixels. */
	void SetSdfSize(int size);
	
	/** Sdf pixel range. */
	inline double GetSdfPixelRange() const{ return pSdfPixelRange; }
	
	/** Set Sdf pixel range. */
	void SetSdfPixelRange(double range);
	
	
	/** Calculate glyph layout. If layout failed nullptr is returned. */
	deftLayout::Ref Layout() override;
	
	
	/** SDF unit scale. */
	inline double GetSdfUnitScale() const{ return pUnitScale; }
	
	/** SDF distance range. */
	inline double GetSdfMinVal() const{ return pSdfMinVal; }
	inline double GetSdfMaxVal() const{ return pSdfMaxVal; }
	decVector2 GetSdfDistanceRange() const;
	/*@}*/
	
	
protected:
	/**
	 * Calculate SDF layout parameters.
	 * 
	 * Sets these member variables:
	 * - pLineHeight
	 * - pLineHeightMargin
	 * - pBoundaryBox
	 * - pUnitScale
	 * - pBaseLineMargin
	 * - pRealLineHeight
	 * - pSdfMinVal
	 * - pSdfMaxVal
	 */
	virtual void pCalcSdfLayoutParameters();
	
	/**
	 * Calculate bearing offset and base line.
	 * 
	 * Sets these member variables:
	 * - pBearingOffset
	 * - pBaseLine
	 */
	void pCalcBearingOffsetAndBaseLine() override;
	
	/**
	 * Debug print layout parameters after calculation step.
	 */
	void pDebugPrintLayout() const override;
	
	/**
	 * Initialize glyphs in the given layout.
	 * 
	 * Requires:
	 * - pBaseLine
	 */
	void pInitGlyphs(deftLayout &layout) override;
	
	/**
	 * Layout glyphs in the given layout. Returns true if layout succeeded.
	 */
	bool pLayoutGlyphs(deftLayout &layout) override;
};

#endif
