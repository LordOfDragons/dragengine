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
#include "layout/deftLayouter.h"
#include "layout/deftLayouterSdf.h"
#include "renderer/deftRenderer.h"
#include "renderer/deftRendererSdf.h"

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
#include <dragengine/threading/deMutexGuard.h>

#include "includeMsdfgen.h"



// Class deftFont
///////////////////

// Constructor, destructor
////////////////////////////

deftFont::deftFont(deftModule &module, decBaseFileReader &reader, const deFont::Ref &font) :
pModule(module),
pReader(reader),
pFont(font),
pFace(nullptr),
pStreamRec{},
pOpenArgs{}{
}

deftFont::~deftFont(){
	if(pFace){
		FT_Done_Face(pFace);
	}
}


// Management
///////////////

void deftFont::LoadFont(){
	// pFont->SetImagePath("");
	pFont->SetImage(pModule.GetGameEngine()->GetImageManager()->LoadDefault());
	
	// try loading the cached font data
	pCachedFont = pModule.GetCaches()->LoadFont(pFont);
	if(pCachedFont){
		// loading the cache has initialized the font data already
		return;
	}
	
	// no valid cache found. load font and save new cache
	pOpenFontFile();
	
	pFont->SetLineHeight(16);
	pFont->SetBaseLine(16 * 3 / 4);
	pFont->SetIsColorFont(false);//FT_HAS_COLOR(pFace));
	pFont->SetScalable(FT_IS_SCALABLE(pFace));
	
	if(pFont->GetScalable() && pModule.GetCreateSdfFont()){
		pFont->SetIsSdf(true);
		
	}else if(FT_HAS_FIXED_SIZES(pFace)){
		for(FT_Int i=0; i<pFace->num_fixed_sizes; i++){
			pFont->GetFixedSizes().Add((int)pFace->available_sizes[i].height);
		}
	}
	
	pInitGlyphs();
	
	pCachedFont = deftCachedFont::Ref::New(pModule, pFont);
	pModule.GetCaches()->SaveFont(pCachedFont);
}

deFontSize::Ref deftFont::LoadFontSize(int lineHeight){
	if(pFont->GetIsSdf()){
		return pLoadFontSizeSdf();
		
	}else{
		return pLoadFontSizeBitmap(lineHeight);
	}
	
	// DEBUG
	/*
	if(pComponentCount == 4){
		if(imageSize.z > 1){
			pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png3d");
		}else{
			pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png");
		}
	}
	*/
	// DEBUG
	
	// if done on manually created images data is lost and can not be retained for graphic module
	// image->ReleaseImageData();
}


// Private functions
//////////////////////

deFontSize::Ref deftFont::pLoadFontSizeBitmap(int lineHeight){
	const char * const filename = pFont->GetFilename();
	
	// check request line height. can use an already loaded size
	deFontSize::Ref fontSize;
	
	const int lineHeightLimit = pModule.GetLineHeightLimit();
	if(lineHeight > lineHeightLimit){
		pModule.LogInfoFormat("LoadFontSize[%s,%d]: Request font size too large. Using %d instead",
			filename, lineHeight, lineHeightLimit);
		fontSize = pFont->GetSizeWith(lineHeightLimit);
		if(fontSize){
			return fontSize;
		}
		
		lineHeight = lineHeightLimit;
	}
	
	// try loading the cached font size data
	fontSize = deFontSize::Ref::New(lineHeight, pFont->GetGlyphCount());
	
	pCachedFontSize = pModule.GetCaches()->LoadFontSize(pFont, fontSize);
	if(pCachedFontSize){
		// loading the cache has initialized the font size data already
		pModule.LogInfoFormat("LoadFontSize[%s,%d]: Loaded from cache, %d glyphs, image size (%d,%d,%d)",
			filename, lineHeight, fontSize->GetGlyphs().GetCount(), fontSize->GetImage()->GetWidth(),
			fontSize->GetImage()->GetHeight(), fontSize->GetImage()->GetDepth());
		
		return fontSize;
	}
	
	// no valid cache found. build font size and save new cache
	pOpenFontFile();
	
	FT_Int32 loadFlags;
	int componentCount;
	if(FT_HAS_COLOR(pFace)){
		loadFlags = FT_LOAD_DEFAULT | FT_LOAD_COLOR;
		componentCount = 4;
		
	}else{
		loadFlags = FT_LOAD_COLOR;
		componentCount = 2;
	}
	
	deftLayouter layouter(pModule, pFont, fontSize, pFace);
	layouter.SetLoadFlags(loadFlags);
	layouter.SetMaxImageWidth(pModule.GetMaxImageSize());
	layouter.SetGlyphSpacing(pModule.GetGlyphSpacing());
	
	deftLayout::Ref layout = layouter.Layout();
	layout->Apply(fontSize);
	
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Layout calculated, %d glyphs, image size (%d,%d,%d)",
		filename, lineHeight, layout->GetGlyphs().GetCount(),
		layout->GetImageSize().x, layout->GetImageSize().y, layout->GetImageSize().z);
	
	// pDebugPrintGlyphs(size, layout);
	
	decTimer timer;
	deftRenderer renderer(pModule, pFace);
	renderer.SetLoadFlags(loadFlags);
	renderer.SetComponentCount(componentCount);
	renderer.Render(layout, fontSize);
	
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Image rendered in %dms",
		filename, lineHeight, (int)(timer.GetElapsedTime() * 1e3f));
	
	// write cache
	pCachedFontSize = deftCachedFontSize::Ref::New(pModule, pFont, fontSize);
	pModule.GetCaches()->SaveFontSize(pCachedFontSize);
	
	return fontSize;
}

deFontSize::Ref deftFont::pLoadFontSizeSdf(){
	// return first font size if existing. works since SDF requires only one font size.
	// if font file changes the font object itself becomes invalid and we create a new one
	if(pFont->GetSizeCount() > 0){
		return deFontSize::Ref(&pFont->GetSizeAt(0));
	}
	
	const char * const filename = pFont->GetFilename();
	
	deTUniqueReference<deftLayouterSdf> layouter;
	deftLayout::Ref layout;
	int sdfSize = 0;
	
	// determine the best SDF line height to use
	{
	if(!pCachedFont){
		pCachedFont = pModule.GetCaches()->LoadFont(pFont);
		if(!pCachedFont){
			pCachedFont = deftCachedFont::Ref::New(pModule, pFont);
		}
	}
	
	const deMutexGuard guard(pCachedFont->GetMutex());
	sdfSize = pCachedFont->GetSdfSize();
	
	if(sdfSize == 0){
		// we have not determined the best SDF parameters yet so find the best size
		sdfSize = pSdfFindBestLayout(layouter, layout);
	}
	}
	
	// try loading the cached font size data
	auto fontSize = deFontSize::Ref::New(sdfSize, pFont->GetGlyphCount());
	
	pCachedFontSize = pModule.GetCaches()->LoadFontSize(pFont, fontSize);
	if(pCachedFontSize){
		// loading the cache has initialized the font size data already
		pModule.LogInfoFormat("LoadFontSize[%s,sdf]: Loaded from cache, %d glyphs, "
				"image size (%d,%d,%d), SDF size %d",
			filename, fontSize->GetGlyphs().GetCount(), fontSize->GetImage()->GetWidth(),
			fontSize->GetImage()->GetHeight(), fontSize->GetImage()->GetDepth(), sdfSize);
		
		return fontSize;
	}
	
	// no valid cache found. build font size and save new cache
	if(!layouter){
		// best layout known but cache is invalid. find best layout
		const deMutexGuard guard(pCachedFont->GetMutex());
		sdfSize = pSdfFindBestLayout(layouter, layout);
	}
	
	layout->Apply(fontSize);
	fontSize->SetSdfPixelRange((float)layouter->GetSdfPixelRange());
	fontSize->SetSdfDistanceRange(layouter->GetSdfDistanceRange());
	
	pModule.LogInfoFormat("LoadFontSize[%s,sdf]: Layout calculated, %d glyphs, "
			"image size (%d,%d,%d), SDF size %d, SDF pixel range %g",
		filename, layout->GetGlyphs().GetCount(), layout->GetImageSize().x,
		layout->GetImageSize().y, layout->GetImageSize().z, sdfSize, layouter->GetSdfPixelRange());
	
	// pDebugPrintGlyphs(size, layout);
	
	// render layout
	{
	decTimer timer;
	
	deftRendererSdf renderer(pModule, pFace);
	renderer.SetLoadFlags(layouter->GetLoadFlags());
	renderer.SetPixelRange(layouter->GetSdfPixelRange());
	renderer.SetUnitScale(layouter->GetSdfUnitScale());
	renderer.SetSdfDistanceRange(layouter->GetSdfMinVal(), layouter->GetSdfMaxVal());
	renderer.Render(layout, fontSize);
	
	pModule.LogInfoFormat("LoadFontSize[%s,sdf]: Image rendered in %dms",
		filename, (int)(timer.GetElapsedTime() * 1e3f));
	}
	
	// write cache
	pCachedFontSize = deftCachedFontSize::Ref::New(pModule, pFont, fontSize);
	pModule.GetCaches()->SaveFontSize(pCachedFontSize);
	
	return fontSize;
}

int deftFont::pSdfFindBestLayout(deTUniqueReference<deftLayouterSdf> &layouter, deftLayout::Ref &layout){
	// FT_LOAD_NO_SCALE: ensure coordinates are in exact font design units (not 26.6 fixed-point)
	// FT_LOAD_NO_BITMAP: do not load embedded bitmaps. just in case the font has them
	// FT_LOAD_NO_HINTING: disable hinting to get raw font outlines
	const FT_Int32 loadFlags = FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING;
	
	pOpenFontFile();
	
	layouter = deTUniqueReference<deftLayouterSdf>::New(pModule, pFont, pFace);
	layouter->SetLoadFlags(loadFlags);
	layouter->SetMaxImageWidth(pModule.GetMaxImageSize());
	layouter->SetGlyphSpacing(pModule.GetGlyphSpacing());
	
	{
	decTimer timer;
	
	layout = layouter->FindBestLayout();
	DEASSERT_NOTNULL(layout)
	
	pModule.LogInfoFormat("LoadFontSize[%s,sdf]: Layout calculated in %dms",
		pFont->GetFilename().GetString(), (int)(timer.GetElapsedTime() * 1e3f));
	}
	
	pCachedFont->SetSdfSize(layouter->GetSdfSize());
	pModule.GetCaches()->SaveFont(pCachedFont);
	
	return layouter->GetSdfSize();
}

void deftFont::pOpenFontFile(){
	if(pFace){
		return;
	}
	
	pStreamRec.size = pReader.GetLength();
	pStreamRec.pos = pReader.GetPosition();
	pStreamRec.descriptor.pointer = this;
	pStreamRec.read = fStreamRead;
	pStreamRec.close = fStreamClose;
	
	pOpenArgs.flags = FT_OPEN_STREAM;
	pOpenArgs.stream = &pStreamRec;
	
	pModule.AssertFT(FT_Open_Face(pModule.GetFTLibrary(), &pOpenArgs, 0, &pFace), "FT_Open_Face");
	pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, 16), "FT_Set_Pixel_Sizes");
}

int deftFont::pCountUsableGlyphs() const{
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	int glyphCount = 0;
	
	while(gindex != 0){
		if(charcode < 0xffff){
			glyphCount++;
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
	
	return glyphCount;
}

void deftFont::pInitGlyphs() const{
	pFont->SetGlyphCount(pCountUsableGlyphs());
	
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	while(gindex != 0){
		if(charcode < 0xffff){
			pFont->GetGlyphAt(glyphIndex++).SetUnicode((int)charcode);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

void deftFont::pDebugPrintGlyphs(const deFontSize &size, const deftLayout &layout) const{
	const char * const filename = pFont->GetFilename();
	const int lineHeight = size.GetLineHeight();
	
	layout.GetGlyphs().Visit([&](const deftLayout::Glyph &g){
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d].Glyph: u=0x%x s=(%d,%d) b=(%d,%d) a=%d tc=(%d,%d,%d)",
			filename, lineHeight, g.unicode, g.size.x, g.size.y, g.bearing.x, g.bearing.y,
			g.advance, g.position.x, g.position.y, g.position.z);
	});
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
