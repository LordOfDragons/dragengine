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

#include "deftRenderer.h"
#include "../deftModule.h"
#include "../layout/deftLayout.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/image/deImageManager.h>


struct sFTColorBGRA{
	unsigned char blue, green, red, alpha;
};


// Class deftRenderer
///////////////////////

// Constructor, destructor
////////////////////////////

deftRenderer::deftRenderer(deftModule &module, FT_Face face) :
pModule(module),
pFace(face),
pLoadFlags(FT_LOAD_DEFAULT),
pComponentCount(2){
}

deftRenderer::~deftRenderer() = default;


// Management
///////////////

void deftRenderer::SetLoadFlags(FT_Int32 loadFlags){
	pLoadFlags = loadFlags;
}

void deftRenderer::SetComponentCount(int componentCount){
	switch(componentCount){
	case 2:
	case 4:
		pComponentCount = componentCount;
		break;
		
	default:
		DETHROW_INFO(deeInvalidParam, "Invalid component count");
	}
}


void deftRenderer::Render(const deftLayout &layout, deFontSize &fontSize){
	const auto &imageSize = layout.GetImageSize();
	const auto image = pModule.GetGameEngine()->GetImageManager()->CreateImage(
		imageSize.x, imageSize.y, imageSize.z, pComponentCount, 8);
	
	if(pComponentCount == 2){
		pRenderNonColor(layout, image);
		
	}else if(pComponentCount == 4){
		pRenderColor(layout, image);
		
	}else{
		return;
	}
	
	image->NotifyImageDataChanged();
	fontSize.SetImage(image);
}


// Protected Functions
////////////////////////

void deftRenderer::pRenderNonColor(const deftLayout &layout, deImage &image){
	const auto &imageSize = layout.GetImageSize();
	const auto &glyphs = layout.GetGlyphs();
	
	const int pixelCount = imageSize.x * imageSize.y * imageSize.z;
	const int strideZ = imageSize.x * imageSize.y;
	const int strideY = imageSize.x;
	
	int x, y, glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	auto di = image.GetDataGrayscaleAlpha8();
	for(x=0; x<pixelCount; x++){
		di[x] = {255, 0};
	}
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
			
			const deftLayout::Glyph &glyph = glyphs[glyphIndex++];
			
			if(pFace->glyph->bitmap.buffer){
				const int rows = pFace->glyph->bitmap.rows;
				
				// hinting can cause the glyph to be slightly larger than the boundary box.
				// since we can not enlarge the image anymore at this point in time we have
				// to clip the glyph. using the bearing the glyph lines up at the top. hence
				// the best we can do is clip the glyph at the bottom. this could be improved
				// by adding a clip area at the bottom of the line as safeguard
				const int safeRows = decMath::min(rows, glyph.size.y);
				const int padTop = rows - safeRows;
				
				const int pitch = pFace->glyph->bitmap.pitch;
				
				const unsigned char *bdBase = pFace->glyph->bitmap.buffer;
				if(pitch < 0){
					bdBase += (rows - 1 - padTop) * -pitch;
					
				}else{
					bdBase += padTop * pitch;
				}
				
				// some fonts cause the bearing to be included in the bitmap width although it
				// is not included in the metrics. this is incorrect and causes problems.
				// as a work around calculate the padding on the left side and clip the bitmap
				const unsigned char *bd = bdBase;
				const int realWidth = pFace->glyph->bitmap.width;
				int padLeft = realWidth - 1;
				
				for(y=0; y<safeRows; y++){
					for(x=0; x<realWidth; x++){
						if(bd[x] > 0){
							padLeft = decMath::min(padLeft, x);
							break;
						}
					}
					bd += pitch;
				}
				
				const int width = decMath::min(realWidth - padLeft, glyph.size.x);
				
				/*
				pModule.LogInfoFormat("Render: g=0x%x s=(%d,%d) p=%d pm=%d pad=%d", (int)charcode,
					pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows,
					pFace->glyph->bitmap.pitch, pFace->glyph->bitmap.pixel_mode, padLeft);
				*/
				
				sGrayscaleAlpha8 *dg = di + strideZ * glyph.position.z
					+ strideY * glyph.position.y + glyph.position.x;
				bd = bdBase + padLeft;
				
				for(y=0; y<safeRows; y++){
					for(x=0; x<width; x++){
						dg[x].alpha = bd[x];
					}
					dg += strideY;
					bd += pitch;
				}
			}
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

void deftRenderer::pRenderColor(const deftLayout &layout, deImage &image){
	const auto &imageSize = layout.GetImageSize();
	const auto &glyphs = layout.GetGlyphs();
	
	const int pixelCount = imageSize.x * imageSize.y * imageSize.z;
	const int strideZ = imageSize.x * imageSize.y;
	const int strideY = imageSize.x;
	
	int x, y, glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	auto di = image.GetDataRGBA8();
	for(x=0; x<pixelCount; x++){
		di[x] = {255, 255, 255, 0};
	}
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
			
			const deftLayout::Glyph &glyph = glyphs[glyphIndex++];
			
			if(pFace->glyph->bitmap.buffer){
				const int rows = pFace->glyph->bitmap.rows;
				
				// hinting can cause the glyph to be slightly larger than the boundary box.
				// since we can not enlarge the image anymore at this point in time we have
				// to clip the glyph. using the bearing the glyph lines up at the top. hence
				// the best we can do is clip the glyph at the bottom. this could be improved
				// by adding a clip area at the bottom of the line as safeguard
				const int safeRows = decMath::min(rows, glyph.size.y);
				const int padTop = rows - safeRows;
				
				const int pitch = pFace->glyph->bitmap.pitch;
				
				const unsigned char *bdBase = pFace->glyph->bitmap.buffer;
				if(pitch < 0){
					bdBase += (rows - 1 - padTop) * -pitch;
					
				}else{
					bdBase += padTop * pitch;
				}
				
				// some fonts cause the bearing to be included in the bitmap width although it
				// is not included in the metrics. this is incorrect and causes problems.
				// as a work around calculate the padding on the left side and clip the bitmap
				const unsigned char *bd = bdBase;
				const int realWidth = pFace->glyph->bitmap.width;
				int padLeft = realWidth - 1;
				
				for(y=0; y<safeRows; y++){
					const sFTColorBGRA * const bg = (sFTColorBGRA*)bd;
					for(x=0; x<realWidth; x++){
						if(bg[x].alpha > 0){
							padLeft = decMath::min(padLeft, x);
							break;
						}
					}
					bd += pitch;
				}
				
				const int width = decMath::min(realWidth - padLeft, glyph.size.x);
				
				/*
				pModule.LogInfoFormat("Render: g=0x%x s=(%d,%d) p=%d pm=%d pad=(%d,%d)", (int)charcode,
					pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows,
					pFace->glyph->bitmap.pitch, pFace->glyph->bitmap.pixel_mode, padLeft, padTop);
				*/
				
				auto dg = di + strideZ * glyph.position.z + strideY * glyph.position.y + glyph.position.x;
				bd = bdBase + sizeof(sFTColorBGRA) * padLeft;
				
				for(y=0; y<safeRows; y++){
					const sFTColorBGRA * const bg = (sFTColorBGRA*)bd;
					for(x=0; x<width; x++){
						dg[x].red = bg[x].red;
						dg[x].green = bg[x].green;
						dg[x].blue = bg[x].blue;
						dg[x].alpha = bg[x].alpha;
					}
					dg += strideY;
					bd += pitch;
				}
			}
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}
