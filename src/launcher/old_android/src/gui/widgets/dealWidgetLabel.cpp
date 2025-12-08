/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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
 
#include <stdio.h>

#include "dealWidgetLabel.h"
#include "../dealDisplay.h"
#include "../dealFont.h"
#include "../dealImage.h"
#include "../dealShader.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../common/string/unicode/decUTF8Decoder.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Class dealWidgetLabel
//////////////////////////

// Constructors, destructors
//////////////////////////////

dealWidgetLabel::dealWidgetLabel(dealDisplay &display, const char *text) :
dealWidget(display),
pFontSize(0),
pText(text),
pColor(0.0f, 0.0f, 0.0f),
pAlignment(eaCenter | eaMiddle),
pMaxLineWidth(GetDisplay().GetWidth() / 2),
pDirtyTextSize(true)
{
}

dealWidgetLabel::dealWidgetLabel(dealDisplay &display, dealFont::Ref font, int fontSize, const char *text) :
dealWidget(display),
pFontSize(0),
pText(text),
pColor(0.0f, 0.0f, 0.0f),
pAlignment(eaCenter | eaMiddle),
pMaxLineWidth(GetDisplay().GetWidth() / 2),
pDirtyTextSize(true)
{
	SetFont(font);
	SetFontSize(fontSize);
}

dealWidgetLabel::~dealWidgetLabel(){
}



// Management
///////////////

void dealWidgetLabel::SetFont(dealFont::Ref font){
	if(font == pFont){
		return;
	}
	pFont = font;
	pDirtyTextSize = true;
	DirtyParentLayout();
}

void dealWidgetLabel::SetFontSize(int fontSize){
	if(fontSize == pFontSize){
		return;
	}
	
	pFontSize = decMath::max(fontSize, 0);
	
	pDirtyTextSize = true;
	DirtyParentLayout();
}

void dealWidgetLabel::SetText(const char *text){
	if(pText == text){
		return;
	}
	
	pText = text;
	
	pDirtyTextSize = true;
	DirtyParentLayout();
}

void dealWidgetLabel::SetColor(const decColor &color){
	if(pColor.IsEqualTo(color)){
		return;
	}
	
	pColor = color;
	
	pDirtyTextSize = true;
	DirtyParentLayout();
}

void dealWidgetLabel::SetAlignment(eAlignments alignment){
	if(pAlignment == alignment){
		return;
	}
	
	pAlignment = alignment;
	
	pDirtyTextSize = true;
	DirtyParentLayout();
}

void dealWidgetLabel::SetMaxLineWidth(int lineWidth){
	if(lineWidth < 0){
		DETHROW(deeInvalidParam);
	}
	
	pMaxLineWidth = lineWidth;
	
	pDirtyTextSize = true;
	DirtyParentLayout();
}



decPoint dealWidgetLabel::GetMinimumSize(){
	if(GetHasExplicitMinimumSize()){
		return GetExplicitMinimumSize();
	}
	
	UpdateTextSize();
	
	return pTextSize + decPoint(GetPaddingLeft() + GetPaddingRight(), GetPaddingTop() + GetPaddingBottom());
}



void dealWidgetLabel::RenderContent(const sRenderContext &context){
	dealShader &shader = *GetDisplay().GetTexturedShapeShader();
	const decPoint &size = GetSize();
	
	shader.Activate();
	
	decPoint position;
	
	switch(pAlignment & 0xf0){
	case eaTop:
		position.y = GetPaddingTop();
		break;
		
	case eaBottom:
		position.y = size.y - GetPaddingBottom() - pTextSize.y;
		break;
		
	default: // eaMiddle
		position.y = GetPaddingTop() + (size.y - GetPaddingTop() - GetPaddingBottom() - pTextSize.y) / 2;
	}
	
	if(GetEnabled()){
		ShaderSetColorMatrix(shader, pColor);
		
	}else{
		ShaderSetColorMatrixDisabled(shader, pColor);
	}
	
	ShaderSetGamma(shader, 1.0f);
	ShaderSetClipRect(context, shader);
	ShaderSetTCClamp(shader);
	
	dealFont &font = pFont ? *pFont : *GetDisplay().GetDefaultFont();
	const int fontSize = pFontSize > 0 ? pFontSize : GetDisplay().GetDefaultFontSize();
	const float scaleFactor = (float)fontSize / (float)font.GetLineHeight();
	const float pixelSizeU = 1.0f / (float)font.GetImage()->GetWidth();
	const float pixelSizeV = 1.0f / (float)font.GetImage()->GetHeight();
	const float lineSizeV = pixelSizeV * (float)font.GetLineHeight();
	const int availWidth = size.x - GetPaddingLeft() - GetPaddingRight();
	decPoint glyphSize(0, fontSize);
	decTexMatrix2 tcmatrix;
	decUTF8Decoder decoder;
	
	BindTexture(0, *font.GetImage(), true, false);
	
	decoder.SetString(pText);
	
	while(!decoder.HasReachedEnd()){
		const int lineStart = decoder.GetPosition();
		int lineEnd = lineStart;
		
		// determine line width
		int lineWidth = 0;
		int wordStart = -1;
		int wordWidth = 0;
		
		while(true){
			if(wordStart == -1){
				wordStart = decoder.GetPosition();
			}
			
			const int character = decoder.DecodeNextCharacter();
			
			if(character == ' ' || character == '\t' || character == '-'
			|| character == '/' || character == '\\' || character == '\n' || character == -1){
				if(lineWidth > 0 && lineWidth + wordWidth > availWidth){
					lineEnd = wordStart;
					break;
				}
				
				lineWidth += wordWidth;
				wordStart = -1;
				wordWidth = 0;
			}
			
			if(character == '\n' || character == -1){
				lineEnd = decoder.GetPosition();
				break;
			}
			
			wordWidth += (int)((float)font.GetGlyphFor(character).advance * scaleFactor);
		}
		
		// align line
		switch(pAlignment & 0x0f){
		case eaLeft:
			position.x = GetPaddingLeft();
			break;
			
		case eaRight:
			position.x = size.x - GetPaddingRight() - lineWidth;
			break;
			
		default: // eaCenter
			position.x = GetPaddingLeft() + (size.x - GetPaddingLeft() - GetPaddingRight() - lineWidth) / 2;
		}
		
		// render line
		decoder.SetPosition(lineStart);
		
		while(decoder.GetPosition() < lineEnd){
			const dealFont::sGlyph &glyph = font.GetGlyphFor(decoder.DecodeNextCharacter());
			
			glyphSize.x = (int)((float)glyph.width * scaleFactor);
			ShaderSetTransform(context, shader, position, glyphSize);
			
			tcmatrix.SetST(pixelSizeU * (float)glyph.width, lineSizeV,
				pixelSizeU * (float)glyph.u, pixelSizeV * (float)glyph.v);
			ShaderSetTCTransform(shader, tcmatrix);
			
			DrawRectangle();
			
			position.x += (int)((float)glyph.advance * scaleFactor);
		}
		
		position.y += fontSize;
	}
	
	OGL_CHECK(GetDisplay().GetLauncher(), glBindTexture(GL_TEXTURE_2D, 0));
}



void dealWidgetLabel::UpdateTextSize(){
	if(!pDirtyTextSize){
		return;
	}
	
	const dealFont &font = pFont ? *pFont : *GetDisplay().GetDefaultFont();
	const int fontSize = pFontSize > 0 ? pFontSize : GetDisplay().GetDefaultFontSize();
	const float scaleFactor = (float)fontSize / (float)font.GetLineHeight();
	decUTF8Decoder decoder;
	int lineWidth = 0;
	int wordWidth = 0;
	
	pTextSize.Set(0, 0);
	
	decoder.SetString(pText);
	
	while(true){
		const int character = decoder.DecodeNextCharacter();
		
		if(character == ' ' || character == '\t' || character == '-'
		|| character == '/' || character == '\\' || character == '\n' || character == -1){
			if(lineWidth > 0 && lineWidth + wordWidth > pMaxLineWidth){
				pTextSize.x = decMath::max(pTextSize.x, lineWidth);
				pTextSize.y += fontSize;
				lineWidth = 0;
				
			}else{
				lineWidth += wordWidth;
			}
			
			wordWidth = 0;
		}
		
		if(character == '\n' || character == -1){
			pTextSize.x = decMath::max(pTextSize.x, lineWidth + wordWidth);
			pTextSize.y += fontSize;
			lineWidth = 0;
			wordWidth = 0;
		}
		
		if(character == '\n'){
			continue;
			
		}else if(character == -1){
			break;
		}
		
		const dealFont::sGlyph &glyph = font.GetGlyphFor(character);
		const int charWidth = (int)((float)glyph.advance * scaleFactor);
		wordWidth += charWidth;
	}
	
	pDirtyTextSize = false;
}
