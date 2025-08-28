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

#include "deftFont.h"
#include "deftModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>


// Class deftFont
///////////////////

// Constructor, destructor
////////////////////////////

deftFont::deftFont(deftModule &module, decBaseFileReader &reader) :
pModule(module),
pReader(reader),
pFace(nullptr),
pStreamRec{},
pOpenArgs{},
pMaxImageWidth(2048),
pGlyphSpacing(1),
pLineHeightLimit(128),
pRealFontSize(0),
pBaseLine(0),
pRealLineHeight(0)
{
	pStreamRec.size = reader.GetLength();
	pStreamRec.pos = reader.GetPosition();
	pStreamRec.descriptor.pointer = this;
	pStreamRec.read = fStreamRead;
	pStreamRec.close = fStreamClose;
	
	pOpenArgs.flags = FT_OPEN_STREAM;
	pOpenArgs.stream = &pStreamRec;
	
	module.AssertFT(FT_Open_Face(module.GetFTLibrary(), &pOpenArgs, 0, &pFace), "FT_Open_Face");
}

deftFont::~deftFont(){
	if(pFace){
		FT_Done_Face(pFace);
	}
}


// Management
///////////////

void deftFont::LoadFont(deFont &font){
	pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, 16), "FT_Set_Pixel_Sizes");
	
	font.SetImagePath("");
	font.SetImage(deImage::Ref::New(pModule.GetGameEngine()->GetImageManager()->LoadDefault()));
	
	font.SetLineHeight(16);
	font.SetBaseLine(16 * 3 / 4);
	font.SetIsColorFont(FT_HAS_COLOR(pFace));
	font.SetScalable(FT_IS_SCALABLE(pFace));
	
	if(FT_HAS_FIXED_SIZES(pFace)){
		FT_Int i;
		for(i=0; i<pFace->num_fixed_sizes; i++){
			font.GetFixedSizes().Add((int)pFace->available_sizes[i].height);
		}
	}
	
	pInitGlyphs(font);
}

deFontSize::Ref deftFont::LoadFontSize(deFont &font, int lineHeight){
	const char * const filename = font.GetFilename();
	deFontSize::Ref size;
	
	if(lineHeight > pLineHeightLimit){
		pModule.LogInfoFormat("LoadFontSize[%s,%d]: Request font size too large. Using %d instead",
			filename, lineHeight, pLineHeightLimit);
		size = font.GetSizeWith(pLineHeightLimit);
		if(size){
			return size;
		}
		
		lineHeight = pLineHeightLimit;
	}
	
	size.TakeOver(new deFontSize(lineHeight, font.GetGlyphCount()));
	
	pRealFontSize = pFindBestSize(font, size);
	
	pRealLineHeight = pBBox.y2 - pBBox.y1;
	
	pBearingOffset.x = 0;
	pBearingOffset.y = (lineHeight - pRealLineHeight) / 2;
	pBaseLine = pBBox.y2 + pBearingOffset.y;
	
	pModule.LogInfoFormat(
		"LoadFontSize[%s,%d]: fs=%d bb=(%d,%d,%d,%d) h=%d gh=%d o=(%d,%d) bl=%d",
		filename, lineHeight, pRealFontSize, pBBox.x1, pBBox.y1, pBBox.x2, pBBox.y2,
		(int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64),
		pRealLineHeight, pBearingOffset.x, pBearingOffset.y, pBaseLine);
	
	pInitGlyphs(size);
	
	decPoint3 imageSize(pCalcLayout(size));
	DEASSERT_FALSE(imageSize == decPoint3())
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Layout calculated, %d glyphs, image size (%d,%d,%d)",
		filename, lineHeight, size->GetGlyphCount(), imageSize.x, imageSize.y, imageSize.z);
	
	// pDebugPrintGlyphs(font, size);
	
	// const bool hasColor(FT_HAS_COLOR(pFace));
	
	decTimer timer;
	const deImage::Ref image(deImage::Ref::New(pModule.GetGameEngine()->GetImageManager()->
		CreateImage(imageSize.x, imageSize.y, imageSize.z, /*hasColor ? 4 : 2*/ 2, 8)));
	pRenderGlyphs(size, image);
	image->NotifyImageDataChanged();
	size->SetImage(image);
	
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Image rendered in %dms",
		filename, lineHeight, (int)(timer.GetElapsedTime() * 1e3f));
	
	// DEBUG
	/*
	if(imageSize.z > 1){
		pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png3d");
	}else{
		pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png");
	}
	*/
	// DEBUG
	
	// if done on manually created images data is lost and can not be retained for graphic module
	// image->ReleaseImageData();
	
	return size;
}


// Private functions
//////////////////////

int deftFont::pCountUsableGlyphs() const{
	int glyphCount = 0;
	
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			glyphCount++;
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
	
	return glyphCount;
}

void deftFont::pInitGlyphs(deFont &font){
	font.SetGlyphCount(pCountUsableGlyphs());
	
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	while(gindex != 0){
		if(charcode < 0xffff){
			deFontGlyph &glyph = font.GetGlyphAt(glyphIndex++);
			glyph.SetUnicode((int)charcode);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

int deftFont::pFindBestSize(const deFont &font, const deFontSize &size){
	// const char * const filename = font.GetFilename();
	const int reqLineHeight = size.GetLineHeight();
	int prevHeight = 0, lineHeight = reqLineHeight;
	
	while(true){
		pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, lineHeight), "FT_Set_Pixel_Sizes");
		
		pBBox.x1 = FT_MulFix(pFace->bbox.xMin, pFace->size->metrics.y_scale) / 64;
		pBBox.y1 = FT_MulFix(pFace->bbox.yMin, pFace->size->metrics.y_scale) / 64;
		pBBox.x2 = FT_MulFix(pFace->bbox.xMax, pFace->size->metrics.y_scale) / 64;
		pBBox.y2 = FT_MulFix(pFace->bbox.yMax, pFace->size->metrics.y_scale) / 64;
		
		const int height = (int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64);
		
		/*
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d]: lh=%d bbox=(%d,%d,%d,%d) height=%d gheight=%d",
			filename, reqLineHeight, lineHeight, pBBox.x1, pBBox.y1, pBBox.x2, pBBox.y2,
			height, pBBox.y2 - pBBox.y1);
		*/
		
		if(height == reqLineHeight){
			return lineHeight;
		}
		
		if(height > reqLineHeight){
			if(prevHeight != 0 && prevHeight < height){
				return lineHeight;
			}
			
			prevHeight = height;
			lineHeight--;
			
		}else{
			prevHeight = height;
			lineHeight++;
		}
	}
}

void deftFont::pInitGlyphs(deFontSize &size){
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, FT_LOAD_DEFAULT), "FT_Load_Glyph");
			
			deFontGlyph &glyph = size.GetGlyphAt(glyphIndex++);
			glyph.SetUnicode((int)charcode);
			glyph.SetWidth(pFace->glyph->metrics.width / 64);
			glyph.SetHeight(pFace->glyph->metrics.height / 64);
			glyph.SetAdvance(pFace->glyph->metrics.horiAdvance / 64);
			
			// bearing in freetype is positive to the right and top.
			// bearing in dragengine is positive to the left and top
			glyph.SetBearing(-(pFace->glyph->metrics.horiBearingX / 64 + pBearingOffset.x));
			glyph.SetBearingY(pFace->glyph->metrics.horiBearingY / 64 - pBaseLine);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

decPoint3 deftFont::pCalcLayout(deFontSize &size){
	if(pRealLineHeight > pMaxImageWidth){
		return decPoint3();
	}
	
	const int glyphCount = size.GetGlyphCount();
	decPoint3 coords, imageSize;
	int i;
	
	for(i=0; i<glyphCount; i++){
		deFontGlyph &glyph = size.GetGlyphAt(i);
		const int glyphWidth = glyph.GetWidth();
		
		if(coords.x + glyphWidth > pMaxImageWidth){
			imageSize.x = decMath::max(imageSize.x, coords.x - pGlyphSpacing);
			coords.x = 0;
			coords.y += pRealLineHeight + pGlyphSpacing;
			
			if(coords.y + pRealLineHeight > pMaxImageWidth){
				imageSize.y = decMath::max(imageSize.y, coords.y - pGlyphSpacing);
				coords.y = 0;
				coords.z++;
			}
		}
		
		glyph.SetX(coords.x);
		glyph.SetY(coords.y + (pRealLineHeight - glyph.GetHeight()) / 2);
		glyph.SetZ(coords.z);
		
		coords.x += glyphWidth + pGlyphSpacing;
	}
	
	imageSize.x = decMath::max(imageSize.x, coords.x - pGlyphSpacing);
	imageSize.y = decMath::max(imageSize.y, coords.y + pRealLineHeight);
	imageSize.z = coords.z + 1;
	
	return imageSize;
}

void deftFont::pDebugPrintGlyphs(const deFont &font, const deFontSize &size){
	const char * const filename = font.GetFilename();
	const int lineHeight = size.GetLineHeight();
	const int glyphCount = size.GetGlyphCount();
	int i;
	
	for(i=0; i<glyphCount; i++){
		const deFontGlyph &g = size.GetGlyphAt(i);
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d].Glyph: u=0x%x s=(%d,%d) b=(%d,%d) a=%d tc=(%d,%d,%d)",
			filename, lineHeight, g.GetUnicode(), g.GetWidth(), g.GetHeight(), g.GetBearing(),
			g.GetBearingY(), g.GetAdvance(), g.GetX(), g.GetY(), g.GetZ());
	}
}

void deftFont::pRenderGlyphs(const deFontSize &size, deImage &image){
	const int pixelCount = image.GetWidth() * image.GetHeight() * image.GetDepth();
	const int strideZ = image.GetWidth() * image.GetHeight();
	const int strideY = image.GetWidth();
	
	int x, y, glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	if(image.GetComponentCount() == 2){
		sGrayscaleAlpha8 * const di = image.GetDataGrayscaleAlpha8();
		for(x=0; x<pixelCount; x++){
			di[x] = {255, 0};
		}
		
		while(gindex != 0){
			if(charcode < 0xffff){
				pModule.AssertFT(FT_Load_Glyph(pFace, gindex, FT_LOAD_DEFAULT), "FT_Load_Glyph");
				pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
				
				const deFontGlyph &glyph = size.GetGlyphAt(glyphIndex++);
				sGrayscaleAlpha8 *dg = di + strideZ * glyph.GetZ() + strideY * glyph.GetY() + glyph.GetX();
				
				const int rows = pFace->glyph->bitmap.rows;
				
				// hinting can cause the glyph to be slightly larger than the boundary box.
				// since we can not enlarge the image anymore at this point in time we have
				// to clip the glyph. using the bearing the glyph lines up at the top. hence
				// the best we can do is clip the glyph at the bottom. this could be improved
				// by adding a clip area at the bottom of the line as safeguard
				// DEASSERT_TRUE(rows <= pRealLineHeight)
				const int safeRows = decMath::min(rows, pRealLineHeight);
				
				const int top = pBaseLine - pFace->glyph->bitmap_top;
				const int bottom = top + safeRows;
				const int width = pFace->glyph->bitmap.width;
				
				const unsigned char *bd = pFace->glyph->bitmap.buffer;
				const int pitch = pFace->glyph->bitmap.pitch;
				if(pitch < 0){
					bd += (rows - 1) * -pitch;
				}
				
				for(y=top; y<bottom; y++){
					for(x=0; x<width; x++){
						dg[x].alpha = bd[x];
					}
					dg += strideY;
					bd += pitch;
				}
			}
			charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
		}
		
	}else if(image.GetComponentCount() == 4){
		sRGBA8 * const di = image.GetDataRGBA8();
		for(x=0; x<pixelCount; x++){
			di[x] = {255, 255, 255, 0};
		}
		
		while(gindex != 0){
			if(charcode < 0xffff){
				pModule.AssertFT(FT_Load_Glyph(pFace, gindex, FT_LOAD_DEFAULT), "FT_Load_Glyph");
				pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
				
				const deFontGlyph &glyph = size.GetGlyphAt(glyphIndex++);
				sRGBA8 *dg = di + strideZ * glyph.GetZ() + strideY * glyph.GetY() + glyph.GetX();
				
				const int rows = pFace->glyph->bitmap.rows;
				
				// hinting can cause the glyph to be slightly larger than the boundary box.
				// since we can not enlarge the image anymore at this point in time we have
				// to clip the glyph. using the bearing the glyph lines up at the top. hence
				// the best we can do is clip the glyph at the bottom. this could be improved
				// by adding a clip area at the bottom of the line as safeguard
				// DEASSERT_TRUE(rows <= pRealLineHeight)
				const int safeRows = decMath::min(rows, pRealLineHeight);
				
				const int top = pBaseLine - pFace->glyph->bitmap_top;
				const int bottom = top + safeRows;
				const int width = pFace->glyph->bitmap.width;
				
				const unsigned char *bd = pFace->glyph->bitmap.buffer;
				const int pitch = pFace->glyph->bitmap.pitch;
				if(pitch < 0){
					bd += (rows - 1) * -pitch;
				}
				
				for(y=top; y<bottom; y++){
					for(x=0; x<width; x++){
						dg[x].alpha = bd[x];
					}
					dg += strideY;
					bd += pitch;
				}
			}
			charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
		}
	}
}


unsigned long deftFont::fStreamRead(FT_Stream stream, unsigned long offset,
unsigned char *buffer, unsigned long count){
	deftFont &font = *(deftFont*)stream->descriptor.pointer;
	
	if(count == 0){
		try{
			font.pReader.SetPosition((int)offset);
			return 0;
			
		}catch(const deException &e){
			font.pModule.LogException(e);
			return 1;
		}
		
	}else{
		font.pReader.Read(buffer, (int)count);
		return count;
	}
}

void deftFont::fStreamClose(FT_Stream){
}
