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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosFont.h"
#include "../beostoolkit.h"
#include "../../../resources/igdeFont.h"
#include "../../../../engine/igdeEngineController.h"
#include "../../../../environment/igdeEnvironment.h"
#include "../../../../gui/igdeApplication.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontBuilder.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>

#include <cstring>



// Font Builder
/////////////////

class igdeFont_FontBuilder : public deFontBuilder{
private:
	struct sGlyph{
		int code;
		int x;
		int y;
		int width;
		int bearing;
		int advance;
	};
	
	igdeFont &pFont;
	int pFirstCode;
	int pLastCode;
	int pMinImageWidth;
	int pMaxImageWidth;
	int pBorderSize;
	int pEnlargeGlyph;
	
	int pLineHeight;
	decTList<sGlyph> pGlyphs;
	int pImageWidth;
	int pImageHeight;
	deImage::Ref pImage;
	
	
	
public:
	igdeFont_FontBuilder(igdeFont &font) :
	pFont(font),
	pFirstCode(32),
	pLastCode(255),
	pMinImageWidth(64),
	pMaxImageWidth(1024),
	pBorderSize(0),
	pEnlargeGlyph(1),
	pLineHeight(0),
	pImageWidth(0),
	pImageHeight(0){
	}
	
	~igdeFont_FontBuilder() override = default;
	
	
	
	deFont::Ref GenerateFont(){
		const BFont &nativeFont = *((BFont*)pFont.GetNativeFont());
		igdeEnvironment &environment = pFont.GetEnvironment();
		deLogger &logger = *environment.GetLogger();
		font_height fh;
		
		nativeFont.GetHeight(&fh);
		const int ascent = (int)(fh.ascent + 0.5f);
		const int descent = (int)(fh.descent + 0.5f);
		const int spacing = (int)(fh.leading + 0.5f);
		pLineHeight = ascent + descent + pEnlargeGlyph * 2;
		logger.LogInfoFormat("DEIGDE", "Generate font: ascent=%i descent=%i spacing=%i lineHeight=%i",
			ascent, descent, spacing, pLineHeight);
		
		pAddGlyphs();
		pCalcLayout();
		pRenderImage();
		
		return pFont.GetEnvironment().GetEngineController()->GetEngine()->GetFontManager()->CreateFont("", *this);
	}
	
	
	
	void BuildFont(deFont *engFont) override{
		if(!engFont){
			DETHROW(deeInvalidParam);
		}
		
		engFont->SetIsColorFont(false);
		engFont->SetLineHeight(pLineHeight);
		engFont->SetImage(pImage);
		engFont->SetGlyphCount(pGlyphs.GetCount());
		
		pGlyphs.VisitIndexed([&](int i, const sGlyph &glyph){
			deFontGlyph &engGlyph = engFont->GetGlyphAt(i);
			
			engGlyph.SetUnicode(glyph.code);
			engGlyph.SetX(glyph.x);
			engGlyph.SetY(glyph.y);
			engGlyph.SetZ(0);
			engGlyph.SetWidth(glyph.width);
			engGlyph.SetHeight(pLineHeight);
			engGlyph.SetBearing(glyph.bearing);
			engGlyph.SetAdvance(glyph.advance);
		});
	}
	
	
	
private:
	void pAddGlyphs(){
		DEASSERT_TRUE(pGlyphs.IsEmpty())
		
		BFont * const nativeFont = (BFont*)pFont.GetNativeFont();
		font_height fh;
		nativeFont->GetHeight(&fh);
		const int ascent = (int)(fh.ascent + 0.5f);
		decUnicodeString character;
		BBitmap *bitmap = nullptr;
		BView *view = nullptr;
		
		try{
			pGlyphs.EnlargeCapacity(256);
			
			// Create offscreen bitmap and view for glyph rendering
			const int imageHeight = pLineHeight;
			const int drawPoint = imageHeight;
			const int imageWidth = drawPoint * 3; // one line-height in each direction
			
			bitmap = new BBitmap(BRect(0, 0, imageWidth - 1, imageHeight - 1), B_RGBA32, true);
			if(!bitmap || !bitmap->IsValid()){
				DETHROW(deeOutOfMemory);
			}
			
			view = new BView(bitmap->Bounds(), "glyph_renderer", B_FOLLOW_NONE, B_WILL_DRAW);
			bitmap->AddChild(view);
			
			bitmap->Lock();
			view->SetFont(nativeFont);
			view->SetHighColor(255, 255, 255, 255);
			view->SetLowColor(0, 0, 0, 255);
			bitmap->Unlock();
			
			const int bytesPerRow = ((imageWidth * 4) + 3) & ~3;
			
			// Process characters
			int c;
			for(c=pFirstCode; c<=pLastCode; c++){
				// Get character width
				character.SetFromUTF8("");
				character.AppendCharacter(c);
				const decString utf8(character.ToUTF8());
				const int minWidth = (int)(nativeFont->StringWidth(utf8.GetString(), utf8.GetLength()) + 0.5f);
				
				// Clear background and draw glyph
				bitmap->Lock();
				view->SetHighColor(0, 0, 0, 255);
				view->FillRect(view->Bounds());
				view->SetHighColor(255, 255, 255, 255);
				view->DrawString(utf8.GetString(), BPoint(drawPoint, ascent + pEnlargeGlyph));
				view->Sync();
				bitmap->Unlock();
				
				// Analyze pixel data to find left and right bearings
				const uint8 *bits = (const uint8*)bitmap->Bits();
				int leftBearing = 0;
				int rightBearing = imageWidth - 1;
				
				// Find left edge
				for(leftBearing=0; leftBearing<drawPoint; leftBearing++){
					int y;
					for(y=0; y<imageHeight; y++){
						const uint8 *pixel = bits + y * bytesPerRow + leftBearing * 4;
						if(pixel[0] != 0 || pixel[1] != 0 || pixel[2] != 0){
							break; // Found left edge
						}
					}
					if(y < imageHeight){
						break;
					}
				}
				
				// Find right edge
				for(rightBearing=imageWidth-1; rightBearing>drawPoint+minWidth; rightBearing--){
					int y;
					for(y=0; y<imageHeight; y++){
						const uint8 *pixel = bits + y * bytesPerRow + rightBearing * 4;
						if(pixel[0] != 0 || pixel[1] != 0 || pixel[2] != 0){
							break; // Found right edge
						}
					}
					if(y < imageHeight){
						break;
					}
				}
				
				// Store glyph data
				pGlyphs.Add({});
				sGlyph &glyph = pGlyphs.Last();
				glyph.code = c;
				glyph.x = 0;
				glyph.y = 0;
				glyph.width = rightBearing - leftBearing + 1 + pEnlargeGlyph * 2;
				glyph.bearing = drawPoint - leftBearing + pEnlargeGlyph;
				glyph.advance = minWidth;
			}
			
			// Clean up
			if(bitmap){
				if(bitmap->Lock()){
					bitmap->RemoveChild(view);
					bitmap->Unlock();
				}
				delete view;
				view = nullptr;
				delete bitmap;
				bitmap = nullptr;
			}
			
		}catch(const deException &){
			if(bitmap){
				if(bitmap->Lock()){
					if(view){
						bitmap->RemoveChild(view);
						delete view;
					}
					bitmap->Unlock();
				}
				delete bitmap;
			}
			throw;
		}
	}
	
	void pCalcLayout(){
		deLogger &logger = *pFont.GetEnvironment().GetLogger();
		
		// Determine the width of all glyphs placed on one line
		int fullWidth = pBorderSize;
		int maxGlyphWidth = 0;
		
		pGlyphs.Visit([&](const sGlyph &glyph){
			maxGlyphWidth = decMath::max(maxGlyphWidth, glyph.width);
			fullWidth += glyph.width + pBorderSize;
		});
		
		logger.LogInfoFormat("DEIGDE", "Calculate layout: fullWidth=%i maxGlyphWidth=%i", fullWidth, maxGlyphWidth);
		maxGlyphWidth = (int)((float)maxGlyphWidth * 0.75f);
		
		// Determine smallest image width where glyphs can wrap without exceeding width as height
		pImageHeight = 0;
		for(pImageWidth=pMinImageWidth; pImageWidth<=pMaxImageWidth; pImageWidth<<=1){
			pImageHeight = pLineHeight * ((fullWidth - maxGlyphWidth) / pImageWidth + 1);
			logger.LogInfoFormat("DEIGDE", "Testing image size %ix%i", pImageWidth, pImageHeight);
			if(pImageHeight <= pImageWidth) break;
		}
		logger.LogInfoFormat("DEIGDE", "Using image size %ix%i", pImageWidth, pImageHeight);
		
		// Layout the glyphs using the found image width
		int u = pBorderSize;
		int v = pBorderSize;
		
		pGlyphs.Visit([&](sGlyph &glyph){
			const int width = glyph.width;
			
			if(u + width > pImageWidth){
				u = pBorderSize;
				v += pLineHeight + pBorderSize;
			}
			
			glyph.x = u;
			glyph.y = v;
			
			u += width + pBorderSize;
		});
		
		// Determine matching image height
		pImageHeight = v + pLineHeight + pBorderSize;
		logger.LogInfoFormat("DEIGDE", "Final image size %ix%i", pImageWidth, pImageHeight);
	}
	
	void pRenderImage(){
		BFont * const nativeFont = (BFont*)pFont.GetNativeFont();
		decUnicodeString character;
		BBitmap *bitmap = nullptr;
		BView *view = nullptr;
		
		const int pixelCount = pImageWidth * pImageHeight;
		font_height fh;
		nativeFont->GetHeight(&fh);
		const int ascent = (int)(fh.ascent + 0.5f);
		
		try{
			pImage = pFont.GetEnvironment().GetEngineController()->GetEngine()
				->GetImageManager()->CreateImage(pImageWidth, pImageHeight, 1, 4, 8);
			
			// Create offscreen bitmap for rendering
			bitmap = new BBitmap(BRect(0, 0, pImageWidth - 1, pImageHeight - 1), B_RGBA32, true);
			if(!bitmap || !bitmap->IsValid()){
				DETHROW(deeOutOfMemory);
			}
			
			view = new BView(bitmap->Bounds(), "font_renderer", B_FOLLOW_NONE, B_WILL_DRAW);
			bitmap->AddChild(view);
			
			bitmap->Lock();
			
			// Clear to black
			view->SetHighColor(0, 0, 0, 255);
			view->FillRect(view->Bounds());
			
			// Render all glyphs
			view->SetFont(nativeFont);
			view->SetHighColor(255, 255, 255, 255);
			
			pGlyphs.Visit([&](const sGlyph &glyph){
				const int x = glyph.x + glyph.bearing + pEnlargeGlyph;
				const int y = glyph.y + ascent + pEnlargeGlyph;
				
				character.SetFromUTF8("");
				character.AppendCharacter(glyph.code);
				const decString utf8 = character.ToUTF8();
				view->DrawString(utf8.GetString(), BPoint(x, y));
			});
			
			view->Sync();
			bitmap->Unlock();
			
			// Copy bitmap data to engine image
			const uint8 *bits = (const uint8*)bitmap->Bits();
			const int bytesPerRow = ((pImageWidth * 4) + 3) & ~3;
			
			if(pImage->GetBitCount() == 8){
				sRGBA8 *data = pImage->GetDataRGBA8();
				
				for(int y = 0; y < pImageHeight; y++){
					const uint8 *srcRow = bits + y * bytesPerRow;
					for(int x = 0; x < pImageWidth; x++){
						sRGBA8 &pixel = data[y * pImageWidth + x];
						const uint8 *src = srcRow + x * 4;
						
						pixel.red = 255;
						pixel.green = 255;
						pixel.blue = 255;
						pixel.alpha = src[2]; // Use red channel as alpha (BGRA format)
					}
				}
				
			}else if(pImage->GetBitCount() == 16){
				sRGBA16 *data = pImage->GetDataRGBA16();
				
				for(int y = 0; y < pImageHeight; y++){
					const uint8 *srcRow = bits + y * bytesPerRow;
					for(int x = 0; x < pImageWidth; x++){
						sRGBA16 &pixel = data[y * pImageWidth + x];
						const uint8 *src = srcRow + x * 4;
						
						pixel.red = 65535;
						pixel.green = 65535;
						pixel.blue = 65535;
						pixel.alpha = (unsigned short)(src[2] << 8);
					}
				}
				
			}else{ // pImage->GetBitCount() == 32
				sRGBA32 *data = pImage->GetDataRGBA32();
				const float factor = 1.0f / 255.0f;
				
				for(int y = 0; y < pImageHeight; y++){
					const uint8 *srcRow = bits + y * bytesPerRow;
					for(int x = 0; x < pImageWidth; x++){
						sRGBA32 &pixel = data[y * pImageWidth + x];
						const uint8 *src = srcRow + x * 4;
						
						pixel.red = 1.0f;
						pixel.green = 1.0f;
						pixel.blue = 1.0f;
						pixel.alpha = (float)src[2] * factor;
					}
				}
			}
			
			// Prepare image for use
			pImage->NotifyImageDataChanged();
			
			// Clean up
			if(bitmap){
				if(bitmap->Lock()){
					bitmap->RemoveChild(view);
					bitmap->Unlock();
				}
				delete view;
				view = nullptr;
				delete bitmap;
				bitmap = nullptr;
			}
			
		}catch(const deException &){
			if(bitmap){
				if(bitmap->Lock()){
					if(view){
						bitmap->RemoveChild(view);
						delete view;
					}
					bitmap->Unlock();
				}
				delete bitmap;
			}
			throw;
		}
	}
};



// Class igdeNativeBeosFont
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeosFont::igdeNativeBeosFont() :
pOwner(nullptr),
pFont(nullptr){
}

igdeNativeBeosFont::igdeNativeBeosFont(igdeFont &owner) :
pOwner(&owner),
pFont(nullptr){
	// Create BFont based on owner properties
	pFont = new BFont(be_plain_font);
	
	// Set font family/face if specified
	const decString &fontName = owner.GetName();
	if(!fontName.IsEmpty()){
		font_family family;
		font_style style;
		
		// Parse font name (simplified - in reality might need to look up font families)
		// For now, use the name as-is and let BeOS match it
		// DELint-Allow-strncpy (null terminated)
		strncpy(family, fontName.GetString(), B_FONT_FAMILY_LENGTH);
		family[B_FONT_FAMILY_LENGTH] = '\0';
		
		// Determine style based on bold/italic flags
		// DELint-Allow-strncpy (constant strings, null terminated)
		if(owner.GetBold() && owner.GetItalic()){
			strncpy(style, "Bold Italic", B_FONT_STYLE_LENGTH);
		}else if(owner.GetBold()){
			strncpy(style, "Bold", B_FONT_STYLE_LENGTH);
		}else if(owner.GetItalic()){
			strncpy(style, "Italic", B_FONT_STYLE_LENGTH);
		}else{
			strncpy(style, "Regular", B_FONT_STYLE_LENGTH);
		}
		style[B_FONT_STYLE_LENGTH] = '\0';
		
		pFont->SetFamilyAndStyle(family, style);
	}else{
		// Use default font
		*pFont = *be_plain_font;
	}
	
	// Set font size (apply display scaling)
	const float size = igdeApplication::app().DisplayScaled(owner.GetSize());
	pFont->SetSize(size);
	
	// Set font face flags
	uint16 face = 0;
	if(owner.GetBold()){
		face |= B_BOLD_FACE;
	}
	if(owner.GetItalic()){
		face |= B_ITALIC_FACE;
	}
	if(owner.GetUnderline()){
		face |= B_UNDERSCORE_FACE;
	}
	if(owner.GetStrikeThrough()){
		face |= B_STRIKEOUT_FACE;
	}
	if(face != 0){
		pFont->SetFace(face);
	}
}

igdeNativeBeosFont::~igdeNativeBeosFont(){
	if(pFont){
		delete pFont;
	}
}

igdeNativeBeosFont *igdeNativeBeosFont::CreateNativeFont(igdeFont &owner){
	// DELint-Allow-NewWithoutRef
	return new igdeNativeBeosFont(owner);
}

void igdeNativeBeosFont::DestroyNativeFont(){
	delete this;
}



// Management
///////////////

deFont::Ref igdeNativeBeosFont::CreateEngineFont(){
	return igdeFont_FontBuilder(*pOwner).GenerateFont();
}

decPoint igdeNativeBeosFont::TextSize(const char *text) const{
	if(!pFont || !text){
		return decPoint(0, 0);
	}
	
	// Calculate text width using BFont metrics
	// strlen is safe here because text must be null-terminated for BFont::StringWidth
	const int len = (int)strlen(text);
	const float width = pFont->StringWidth(text, len);
	
	// Get font height metrics
	font_height fh;
	pFont->GetHeight(&fh);
	const float height = fh.ascent + fh.descent + fh.leading;
	
	return decPoint((int)(width + 0.5f), (int)(height + 0.5f));
}

#endif
