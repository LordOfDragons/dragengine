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

#include "deftLayouter.h"
#include "../deftFont.h"
#include "../deftModule.h"

#include <dragengine/resources/font/deFontSize.h>


// Class deftLayouter
///////////////////////

// Constructor, destructor
////////////////////////////

deftLayouter::deftLayouter(deftModule &module, const deFont &font,
	const deFontSize &fontSize, FT_Face face) :
pModule(module),
pFont(font),
pFontSize(fontSize),
pFace(face),
pLoadFlags(FT_LOAD_DEFAULT),
pMaxImageWidth(2048),
pGlyphSpacing(1),
pHintingSafetyMargin(1),
pLineHeight(16),
pBaseLine(0),
pLineHeightMargin(0){
}

deftLayouter::~deftLayouter() = default;


// Management
///////////////

void deftLayouter::SetLoadFlags(FT_Int32 loadFlags){
	pLoadFlags = loadFlags;
}

void deftLayouter::SetMaxImageWidth(int maxImageWidth){
	DEASSERT_TRUE(maxImageWidth >= 256)
	pMaxImageWidth = maxImageWidth;
}

void deftLayouter::SetGlyphSpacing(int glyphSpacing){
	DEASSERT_TRUE(glyphSpacing >= 0)
	pGlyphSpacing = glyphSpacing;
}

void deftLayouter::SetHintingSafetyMargin(int margin){
	DEASSERT_TRUE(margin >= 0)
	pHintingSafetyMargin = margin;
}


deftLayout::Ref deftLayouter::Layout(){
	if(pFontSize.GetLineHeight() > pMaxImageWidth){
		return {};
	}
	
	pFindBestFontSizeForLineHeight();
	pCalcBearingOffsetAndBaseLine();
	pDebugPrintLayout();
	return pCreateLayout();
}


// Protected functions
////////////////////////

void deftLayouter::pFindBestFontSizeForLineHeight(){
	// const char * const filename = pFont.GetFilename();
	const int reqLineHeight = pFontSize.GetLineHeight();
	int prevHeight = 0;
	
	pLineHeight = reqLineHeight;
	
	// enlarge glyphs by a small margin to get some wiggle space if hinting enlarges font
	pLineHeightMargin = pHintingSafetyMargin;
	
	while(true){
		pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, pLineHeight), "FT_Set_Pixel_Sizes");
		
		pBoundaryBox.x1 = FT_MulFix(pFace->bbox.xMin, pFace->size->metrics.y_scale) / 64;
		pBoundaryBox.y1 = FT_MulFix(pFace->bbox.yMin, pFace->size->metrics.y_scale) / 64;
		pBoundaryBox.x2 = FT_MulFix(pFace->bbox.xMax, pFace->size->metrics.y_scale) / 64;
		pBoundaryBox.y2 = FT_MulFix(pFace->bbox.yMax, pFace->size->metrics.y_scale) / 64;
		
		const int height = (int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64);
		
		/*
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d]: lh=%d bbox=(%d,%d,%d,%d) height=%d gheight=%d",
			filename, reqLineHeight, lineHeight, pBoundaryBox.x1, pBoundaryBox.y1,
			pBoundaryBox.x2, pBoundaryBox.y2, height, pBoundaryBox.y2 - pBoundaryBox.y1);
		*/
		
		if(height == reqLineHeight){
			return;
		}
		
		if(height > reqLineHeight){
			if(prevHeight != 0 && prevHeight < height){
				return;
			}
			
			prevHeight = height;
			pLineHeight--;
			
		}else{
			prevHeight = height;
			pLineHeight++;
		}
	}
}

void deftLayouter::pCalcBearingOffsetAndBaseLine(){
	pBearingOffset.x = 0;
	pBearingOffset.y = (pFontSize.GetLineHeight() - pLineHeight) / 2;
	pBaseLine = pBoundaryBox.y2 + pBearingOffset.y;
}

void deftLayouter::pInitGlyphs(deftLayout &layout){
	auto &glyphs = layout.GetGlyphs();
	glyphs.SetCount(pFont.GetGlyphCount(), {});
	
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			deftLayout::Glyph &glyph = glyphs[glyphIndex++];
			glyph.unicode = (int)charcode;
			glyph.size.x = pFace->glyph->metrics.width / 64;
			glyph.size.y = decMath::min(pFace->glyph->metrics.height / 64, pLineHeight);
			glyph.advance = pFace->glyph->metrics.horiAdvance / 64;
			
			// bearing in freetype is positive to the right and top.
			// bearing in dragengine is positive to the left and bottom
			glyph.bearing.x = -(pFace->glyph->metrics.horiBearingX / 64 + pBearingOffset.x);
			glyph.bearing.y = pFace->glyph->metrics.horiBearingY / 64 - pBaseLine;
			
			// enlarge glyph by a small margin to get some wiggle space if hinting enlarges font
			glyph.size.x = glyph.size.x + pHintingSafetyMargin;
			glyph.size.y = glyph.size.y + pHintingSafetyMargin;
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

void deftLayouter::pDebugPrintLayout() const{
	pModule.LogInfoFormat(
		"LoadFontSize[%s,%d]: fs=%d bb=(%d,%d,%d,%d) h=%d o=(%d,%d) bl=%d clr=%d",
		pFont.GetFilename().GetString(), pFontSize.GetLineHeight(), pLineHeight,
		pBoundaryBox.x1, pBoundaryBox.y1, pBoundaryBox.x2, pBoundaryBox.y2,
		(int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64),
		pBearingOffset.x, pBearingOffset.y, pBaseLine, FT_HAS_COLOR(pFace));
}

deftLayout::Ref deftLayouter::pCreateLayout(){
	auto layout = deftLayout::Ref::New();
	pInitGlyphs(layout);
	if(!pLayoutGlyphs(layout)){
		return {};
	}
	
	layout->SetImageSize(pImageSize);
	return layout;
}

bool deftLayouter::pLayoutGlyphs(deftLayout &layout){
	decPoint3 coords;
	pImageSize = {};
	
	layout.GetGlyphs().Visit([&](deftLayout::Glyph &glyph){
		if(coords.x + glyph.size.x > pMaxImageWidth){
			pImageSize.x = decMath::max(pImageSize.x, coords.x - pGlyphSpacing);
			coords.x = 0;
			coords.y += pLineHeight + pGlyphSpacing;
			
			if(coords.y + pLineHeight > pMaxImageWidth){
				pImageSize.y = decMath::max(pImageSize.y, coords.y - pGlyphSpacing);
				coords.y = 0;
				coords.z++;
			}
		}
		
		glyph.position = coords;
		glyph.position.y += (pLineHeight - glyph.size.y) / 2;
		
		coords.x += glyph.size.x + pGlyphSpacing;
	});
	
	pImageSize.x = decMath::max(pImageSize.x, coords.x - pGlyphSpacing);
	pImageSize.y = decMath::max(pImageSize.y, coords.y + pLineHeight);
	pImageSize.z = coords.z + 1;
	
	return true;
}
