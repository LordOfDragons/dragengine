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
	
#include "deftLayouterSdf.h"
#include "../deftModule.h"
#include "../includeMsdfgen.h"

#include <dragengine/resources/font/deFontSize.h>


// Class deftLayouterSdf
//////////////////////////

// Constructor, destructor
////////////////////////////

deftLayouterSdf::deftLayouterSdf(deftModule &module, const deFont &font,
	const deFontSize &fontSize, FT_Face face) :
deftLayouter(module, font, fontSize, face),
pSdfSize(48),
pSdfPixelRange(4.0f),
pBaseLineMargin(0),
pUnitScale(1.0),
pSdfMinVal(0.0),
pSdfMaxVal(1.0){
}

deftLayouterSdf::~deftLayouterSdf() = default;


// Management
///////////////

void deftLayouterSdf::SetSdfSize(int size){
	pSdfSize = decMath::max(size, 8);
}
	
void deftLayouterSdf::SetSdfPixelRange(double range){
	pSdfPixelRange = decMath::max(range, 2.0);
}


deftLayout::Ref deftLayouterSdf::Layout(){
	pCalcSdfLayoutParameters();
	pCalcBearingOffsetAndBaseLine();
	pDebugPrintLayout();
	return pCreateLayout();
}

decVector2 deftLayouterSdf::GetSdfDistanceRange() const{
	return {(float)pSdfMinVal, (float)pSdfMaxVal};
}


// Protected functions
////////////////////////

void deftLayouterSdf::pCalcSdfLayoutParameters(){
	const double targetLineHeight = (double)pFontSize.GetLineHeight();
	pUnitScale = targetLineHeight / (double)pFace->height;
	
	pBoundaryBox.x1 = (int)(std::floor(pFace->bbox.xMin * pUnitScale));
	pBoundaryBox.y1 = (int)(std::floor(pFace->bbox.yMin * pUnitScale));
	pBoundaryBox.x2 = (int)(std::ceil(pFace->bbox.xMax * pUnitScale));
	pBoundaryBox.y2 = (int)(std::ceil(pFace->bbox.yMax * pUnitScale));
	
	const int margin = (int)std::ceil(pSdfPixelRange * 0.5f);
	pBaseLineMargin = margin;
	pLineHeightMargin = margin * 2;
	
	pLineHeight = (pBoundaryBox.y2 - pBoundaryBox.y1) + pLineHeightMargin;
	
	// MTSDF images compose of the MSDF image in the RGB channels and the SDF in the alpha
	// channel. for MSDF a bit depth of 8 is sufficient. for SDF though a bit depth of 8
	// causes the distance values to floor at 0 causing issues with border type effects.
	// to solve this the image has to be increased to 16 or 32 bit to store negative values
	// doing this for the entire image wastes space since MSDF does not require high bit count.
	// but using two images makes the situation more complicate and the graphic module can
	// still decide to upload the image to the GPU in two images if this is better. 16 bit
	// images store the data in the range from 0 to 1 similar to 8 bit images. for the MSDF
	// part of the image this is fine. for the SDF part of the image the values can be negative
	// and larger than 1. for this reason the SDF distances are mapped to the 0 to 1 range.
	
	// the calculation below determines the value range for the worst case situation where
	// pixels all across the diagonal of the cell
	const float maxOuter = pLineHeight * 1.675f;
	const float maxInner = decMath::max(4.0f, pLineHeight * 0.05f);
	
	pSdfMinVal = 0.5f - (maxOuter / pSdfPixelRange);
	pSdfMaxVal = 0.5f + (maxInner / pSdfPixelRange);
}

void deftLayouterSdf::pCalcBearingOffsetAndBaseLine(){
	deftLayouter::pCalcBearingOffsetAndBaseLine();
	pBaseLine += pBaseLineMargin;
}

void deftLayouterSdf::pDebugPrintLayout() const{
	pModule.LogInfoFormat(
		"LoadFontSize[%s,%d,sdf]: bb=(%d,%d,%d,%d) h=%d o=(%d,%d) bl=%d clr=%d m=(%d,%d) spr=%g svmin=%g svmax=%g",
		pFont.GetFilename().GetString(), pFontSize.GetLineHeight(),
		pBoundaryBox.x1, pBoundaryBox.y1, pBoundaryBox.x2, pBoundaryBox.y2,
		(int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64),
		pBearingOffset.x, pBearingOffset.y, pBaseLine, FT_HAS_COLOR(pFace),
		pLineHeightMargin, pBaseLineMargin, pSdfPixelRange, pSdfMinVal, pSdfMaxVal);
}

void deftLayouterSdf::pInitGlyphs(deftLayout &layout){
	auto &glyphs = layout.GetGlyphs();
	glyphs.SetCount(pFont.GetGlyphCount(), {});
	
	const int sdfMargin = (int)std::ceil(pFontSize.GetSdfPixelRange() * 0.5f);
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			const FT_GlyphSlot slot = pFace->glyph;
			deftLayout::Glyph &glyph = glyphs[glyphIndex++];
			
			glyph.unicode = (int)charcode;
			
			if(slot->format == FT_GLYPH_FORMAT_OUTLINE && slot->outline.n_points > 0){
				FT_BBox cbox;
				FT_Outline_Get_CBox(&slot->outline, &cbox);
				
				const int left = (int)std::floor(cbox.xMin * pUnitScale);
				const int right = (int)std::ceil(cbox.xMax * pUnitScale);
				const int bottom = (int)std::floor(cbox.yMin * pUnitScale);
				const int top = (int)std::ceil(cbox.yMax * pUnitScale);
				
				glyph.size.x = right - left + 2 * sdfMargin;
				glyph.size.y = top - bottom + 2 * sdfMargin;
				
				// bearing in dragengine is positive to the left and bottom
				glyph.bearing.x = sdfMargin - left;
				glyph.bearing.y = top + sdfMargin - pBaseLine;
			}
			
			glyph.advance = (int)std::round(slot->metrics.horiAdvance * pUnitScale);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

bool deftLayouterSdf::pLayoutGlyphs(deftLayout &layout){
	return deftLayouter::pLayoutGlyphs(layout) && pImageSize.z <= 1;
}
