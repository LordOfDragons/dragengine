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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feViewFIImage.h"
#include "../feWindowMain.h"
#include "../../configuration/feConfiguration.h"
#include "../../font/feFont.h"
#include "../../font/glyph/feFontGlyph.h"
#include "../../font/image/feFontImage.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class feViewFIImage
////////////////////////

// Constructor, destructor
////////////////////////////

feViewFIImage::feViewFIImage(feWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain),

pBorderSize(2),
pZoom(100){
}

feViewFIImage::~feViewFIImage(){
}



void feViewFIImage::ResetView(){
}


void feViewFIImage::SetFont(feFont *font){
	if(font == pFont){
		return;
	}
	pFont = font;
	ResizeCanvas();
}

void feViewFIImage::SetZoom(int zoom){
	zoom = decMath::clamp(zoom, 100, 8000);
	if(zoom == pZoom){
		return;
	}
	
	pZoom = zoom;
	ResizeCanvas();
}

void feViewFIImage::SetOffset(const decPoint &offset){
	if(offset == pOffset){
		return;
	}
	
	pOffset = offset;
	ResizeCanvas();
}



decPoint feViewFIImage::GetContentSize() const{
	const decPoint borderSize(pBorderSize * 2, pBorderSize * 2);
	
	if(pCanvasFontImage){
		return borderSize + pCanvasFontImage->GetSize();
		
	}else{
		return borderSize;
	}
}



void feViewFIImage::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(!pCanvasImageBackground){
		pCanvasImageBackground.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint());
		pCanvasImageBackground->SetShapeType(deCanvasPaint::estRectangle);
		pCanvasImageBackground->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 1.0f));
		pCanvasImageBackground->SetLineColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
		pCanvasImageBackground->SetThickness(0.0f);
		pCanvasImageBackground->SetOrder(2.0f);
		AddCanvas(pCanvasImageBackground);
	}
	
	if(!pCanvasFontImage){
		pCanvasFontImage.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasImage());
		pCanvasFontImage->SetOrder(3.0f);
		pCanvasFontImage->SetVisible(false);
		AddCanvas(pCanvasFontImage);
	}
	
	if(!pCanvasActiveGlyph){
		pCanvasActiveGlyph.TakeOver(pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint());
		pCanvasActiveGlyph->SetShapeType(deCanvasPaint::estRectangle);
		pCanvasActiveGlyph->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.1f));
		pCanvasActiveGlyph->SetLineColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		pCanvasActiveGlyph->SetThickness(1.0f);
		pCanvasActiveGlyph->SetOrder(4.0f);
		pCanvasActiveGlyph->SetTransparency(0.75f);
		pCanvasActiveGlyph->SetVisible(false);
		AddCanvas(pCanvasActiveGlyph);
	}
}

void feViewFIImage::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pFont){
		pFont->Update(elapsed);
	}
}

void feViewFIImage::OnResize(){
	igdeViewRenderWindow::OnResize();
	ResizeCanvas();
}



void feViewFIImage::ResizeCanvas(){
	if(!GetRenderWindowCanvas()){
		return;
	}
	
	const decPoint &parentSize = GetRenderWindowCanvas()->GetSize();
	
	if(!pFont){
		if(pCanvasFontImage){
			pCanvasFontImage->SetVisible(false);
		}
		if(pCanvasImageBackground){
			pCanvasImageBackground->SetVisible(false);
		}
		if(pCanvasActiveGlyph){
			pCanvasActiveGlyph->SetVisible(false);
		}
		return;
	}
	
	const feFontImage &fontImage = *pFont->GetFontImage();
	deImage * const engImage = fontImage.GetEngineImage();
	
	if(!engImage){
		if(pCanvasFontImage){
			pCanvasFontImage->SetVisible(false);
		}
		if(pCanvasImageBackground){
			pCanvasImageBackground->SetVisible(false);
		}
		if(pCanvasActiveGlyph){
			pCanvasActiveGlyph->SetVisible(false);
		}
		return;
	}
	
	const float zoom = (float)pZoom * 0.01f;
	
	const int imageWidth = (int)((float)engImage->GetWidth() * zoom);
	const int imageHeight = (int)((float)engImage->GetHeight() * zoom);
	const int imageOffsetX = (parentSize.x - imageWidth) / 2 + pOffset.x;
	const int imageOffsetY = (parentSize.y - imageHeight) / 2 + pOffset.y;
	
	if(pCanvasFontImage){
		pCanvasFontImage->SetImage(engImage);
		pCanvasFontImage->SetVisible(true);
		pCanvasFontImage->SetPosition(decPoint(imageOffsetX, imageOffsetY));
		pCanvasFontImage->SetSize(decPoint(imageWidth, imageHeight));
	}
	
	if(pCanvasImageBackground){
		pCanvasImageBackground->SetPosition(pCanvasFontImage->GetPosition());
		pCanvasImageBackground->SetSize(pCanvasFontImage->GetSize());
		pCanvasImageBackground->SetVisible(true);
	}
	
	if(pCanvasActiveGlyph){
		const feFontGlyph * const activeGlyph = pFont->GetGlyphSelection().GetActiveGlyph();
		
		if(activeGlyph){
			pCanvasActiveGlyph->SetVisible(true);
			pCanvasActiveGlyph->SetThickness(zoom);
			
			const int glyphU = activeGlyph->GetU();
			const int glyphV = activeGlyph->GetV();
			const int glyphWidth = activeGlyph->GetWidth();
			const int glyphHeight = pFont->GetLineHeight();
			
			const int x1 = imageOffsetX + (int)((float)glyphU * zoom);
			const int y1 = imageOffsetY + (int)((float)glyphV * zoom);
			const int x2 = imageOffsetX + (int)((float)(glyphU + glyphWidth) * zoom);
			const int y2 = imageOffsetY + (int)((float)(glyphV + glyphHeight) * zoom);
			
			pCanvasActiveGlyph->SetPosition(decPoint(x1, y1));
			pCanvasActiveGlyph->SetSize(decPoint(x2 - x1, y2 - y1));
			
		}else{
			pCanvasActiveGlyph->SetVisible(false);
		}
	}
}
