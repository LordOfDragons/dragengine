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

deftLayouterSdf::deftLayouterSdf(deftModule &module, const deFont &font, FT_Face face) :
deftLayouter(module, font, 48, face),
pSdfSize(48),
pSdfPixelRange(4.0f),
pBaseLineMargin(0),
pUnitScale(1.0),
pSdfMinVal(0.0),
pSdfMaxVal(1.0)
{
	// large font support, high detail fonts with complex strokes
	pTestSdfSizes.Add(128);
	pTestSdfSizes.Add(96);
	pTestSdfSizes.Add(80);
	pTestSdfSizes.Add(72);
	pTestSdfSizes.Add(64);
	pTestSdfSizes.Add(56);
	pTestSdfSizes.Add(48);
	
	// stand font support
	pTestSdfSizes.Add(44);
	pTestSdfSizes.Add(40);
	pTestSdfSizes.Add(36);
	pTestSdfSizes.Add(32);
	pTestSdfSizes.Add(28);
	pTestSdfSizes.Add(24);
	
	// fallback for fonts with large amount of glyphs
	pTestSdfSizes.Add(22);
	pTestSdfSizes.Add(20);
	pTestSdfSizes.Add(18);
	pTestSdfSizes.Add(16);
	
	// test with a blown up font at 240px line height:
	// - pixel range 2-4 does not make much difference in general
	// - lineHeight=48, pixelRange=4:
	//   works good but 'r' and 'p' have slight artifacts in the corners
	// - lineHeight=64, pixelRange=4:
	//   better than 48/4 but 'r' and 'p' are still slightly imperfect
	// - lineHeight=64, pixelRange=6:
	//   pretty much the same as 64/4
	// - lineHeight=80, pixelRange=4:
	//   'r' is now good but 'p' still has a slight imperfection
	// - lineHeight=96, pixelRange=4:
	//   nearly identical to 80/4
	//
	// facit:
	// for blown up font 48 is good enough but 64 can give slightly better results but obviously
	// takes a bit longer to generate and the image is somewhat larger.
	//
	// test with a font at common size of 15px line height:
	// - lineHeight=48, pixelRange=4:
	//   letter like 'r' and 'p' are not showing problems on that scale. a certain blurring
	//   happens due to the smaller amount of pixels covered.
	// - lineHeight=96, pixelRange=4:
	//   similar to 48/4 but slightly less blurring
	//
	// facit:
	// for common font size 48 is good enough but higher works too.
	//
	// test with a small font at 8px line height:
	// - lineHeight=48, pixelRange=4:
	//   difficult to read as the thickness tends to fade out.
	// - lineHeight=96, pixelRange=4:
	//   this actually makes the situation worse due to the stronger sharpening
	//
	// facit:
	// with such small fonts reading text is hard anyway due to the limited amount of pixels
	// available to reach glyph. 48 is a good value here but slightly higher can be acceptable too.
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

void deftLayouterSdf::SetTestSdfSizes(const decTList<int> &sizes){
	pTestSdfSizes = sizes.GetSortedDescending();
}


deftLayout::Ref deftLayouterSdf::FindBestLayout(){
	// pixel range is typically 4 to allow for some border effects and down to 2.5 for high glyph
	// count fonts to get tigher packing. graphic modules are expected to calculate outline effects
	// as independent of the pixel range as possible to avoid locking font modules into estimating
	// the required range. for this reason down to 2.5 would work. we use 3 as a middle ground.
	SetSdfPixelRange(3.0);
	
	// do a binary search over the test SDF sizes starting at the middle. test sizes are sorted
	// from largest to smallest. find the largest size that produces a valid layout
	const char * const filename = pFont.GetFilename();
	int low = 0, high = pTestSdfSizes.GetCount() - 1;
	deftLayout::Ref bestLayout;
	int bestSize = 0;
	
	while(low <= high){
		const int mid = (low + high) / 2;
		const int testSize = pTestSdfSizes.GetAt(mid);
		
		pModule.LogInfoFormat("LoadFontSize[%s,%s,sdf].FindBestSdfSize: Test size %d",
			pFont.GetVirtualFileSystem() ? pFont.GetVirtualFileSystem()->GetIdentifier().GetString() : "-",
			filename, testSize);
		
		SetSdfSize(testSize);
		
		auto layout = Layout();
		if(layout){
			bestLayout = layout;
			bestSize = testSize;
			high = mid - 1; // try larger sizes
			
		}else{
			low = mid + 1; // try smaller sizes
		}
	}
	
	// last tested size may be larger than the best size. restore the state matching the layout
	if(bestLayout && pSdfSize != bestSize){
		SetSdfSize(bestSize);
		bestLayout = Layout();
	}
	
	return bestLayout;
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
	pUnitScale = (double)pSdfSize / (double)pFace->height;
	
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
	pBearingOffset.x = 0;
	pBearingOffset.y = (pSdfSize - pLineHeight) / 2;
	pBaseLine = pBoundaryBox.y2 + pBearingOffset.y + pBaseLineMargin;
}

void deftLayouterSdf::pDebugPrintLayout() const{
	pModule.LogInfoFormat(
		"LoadFontSize[%s,%s,sdf]: bb=(%d,%d,%d,%d) h=%d us=%g o=(%d,%d) bl=%d clr=%d m=(%d,%d) spr=%g svmin=%g svmax=%g",
		pFont.GetVirtualFileSystem() ? pFont.GetVirtualFileSystem()->GetIdentifier().GetString() : "-",
		pFont.GetFilename().GetString(), pBoundaryBox.x1, pBoundaryBox.y1, pBoundaryBox.x2,
		pBoundaryBox.y2, pFace->height, pUnitScale, pBearingOffset.x, pBearingOffset.y, pBaseLine,
		FT_HAS_COLOR(pFace), pLineHeightMargin, pBaseLineMargin, pSdfPixelRange, pSdfMinVal, pSdfMaxVal);
}

void deftLayouterSdf::pInitGlyphs(deftLayout &layout){
	auto &glyphs = layout.GetGlyphs();
	glyphs.SetCount(pFont.GetGlyphCount(), {});
	
	const int sdfMargin = (int)std::ceil(pSdfPixelRange * 0.5f);
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

bool deftLayouterSdf::pVerifyGlyphLayout(const deftLayout&){
	return pImageSize.z <= 1;
}

void deftLayouterSdf::pCalcAvgCharWidth(){
	float ratio;
	
	auto const os2 = (TT_OS2*)FT_Get_Sfnt_Table(pFace, FT_SFNT_OS2);
	if(os2 && os2->xAvgCharWidth > 0 && pFace->units_per_EM > 0){
		ratio = (float)os2->xAvgCharWidth / (float)pFace->units_per_EM;
		
	}else{
		ratio = 0.55f;
	}
	
	pAvgCharWidth = (int)ceilf(ratio * (float)pSdfSize);
}
