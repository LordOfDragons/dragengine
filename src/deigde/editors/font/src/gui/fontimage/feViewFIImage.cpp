/* 
 * Drag[en]gine IGDE Font Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

feViewFIImage::feViewFIImage( feWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),

pFont( NULL ),

pBorderSize( 2 ),
pZoom( 100 ){
}

feViewFIImage::~feViewFIImage(){
	if( pFont ){
		pFont->FreeReference();
	}
}



void feViewFIImage::ResetView(){
}


void feViewFIImage::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
	if( pFont ){
		pFont->FreeReference();
	}
	
	pFont = font;
	
	if( font ){
		font->AddReference();
	}
	
	ResizeCanvas();
}

void feViewFIImage::SetZoom( int zoom ){
	zoom = decMath::clamp( zoom, 100, 8000 );
	if( zoom == pZoom ){
		return;
	}
	
	pZoom = zoom;
	ResizeCanvas();
}

void feViewFIImage::SetOffset( const decPoint &offset ){
	if( offset == pOffset ){
		return;
	}
	
	pOffset = offset;
	ResizeCanvas();
}



decPoint feViewFIImage::GetContentSize() const{
	const decPoint borderSize( pBorderSize * 2, pBorderSize * 2 );
	
	if( pCanvasFontImage ){
		return borderSize + pCanvasFontImage->GetSize();
		
	}else{
		return borderSize;
	}
}



void feViewFIImage::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( ! pCanvasImageBackground ){
		pCanvasImageBackground.TakeOver( pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
		pCanvasImageBackground->SetShapeType( deCanvasPaint::estRectangle );
		pCanvasImageBackground->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
		pCanvasImageBackground->SetLineColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		pCanvasImageBackground->SetThickness( 0.0f );
		pCanvasImageBackground->SetOrder( 2.0f );
		AddCanvas( pCanvasImageBackground );
	}
	
	if( ! pCanvasFontImage ){
		pCanvasFontImage.TakeOver( pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasImage() );
		pCanvasFontImage->SetOrder( 3.0f );
		pCanvasFontImage->SetVisible( false );
		AddCanvas( pCanvasFontImage );
	}
	
	if( ! pCanvasActiveGlyph ){
		pCanvasActiveGlyph.TakeOver( pWindowMain.GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
		pCanvasActiveGlyph->SetShapeType( deCanvasPaint::estRectangle );
		pCanvasActiveGlyph->SetFillColor( decColor( 1.0f, 0.0f, 0.0f, 0.1f ) );
		pCanvasActiveGlyph->SetLineColor( decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
		pCanvasActiveGlyph->SetThickness( 1.0f );
		pCanvasActiveGlyph->SetOrder( 4.0f );
		pCanvasActiveGlyph->SetTransparency( 0.75f );
		pCanvasActiveGlyph->SetVisible( false );
		AddCanvas( pCanvasActiveGlyph );
	}
}

void feViewFIImage::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pFont ){
		pFont->Update( elapsed );
	}
}

void feViewFIImage::OnResize(){
	igdeViewRenderWindow::OnResize();
	ResizeCanvas();
}



void feViewFIImage::ResizeCanvas(){
	if( ! GetRenderWindowCanvas() ){
		return;
	}
	
	const decPoint &parentSize = GetRenderWindowCanvas()->GetSize();
	
	if( ! pFont ){
		if( pCanvasFontImage ){
			pCanvasFontImage->SetVisible( false );
		}
		if( pCanvasImageBackground ){
			pCanvasImageBackground->SetVisible( false );
		}
		if( pCanvasActiveGlyph ){
			pCanvasActiveGlyph->SetVisible( false );
		}
		return;
	}
	
	const feFontImage &fontImage = *pFont->GetFontImage();
	deImage * const engImage = fontImage.GetEngineImage();
	
	if( ! engImage ){
		if( pCanvasFontImage ){
			pCanvasFontImage->SetVisible( false );
		}
		if( pCanvasImageBackground ){
			pCanvasImageBackground->SetVisible( false );
		}
		if( pCanvasActiveGlyph ){
			pCanvasActiveGlyph->SetVisible( false );
		}
		return;
	}
	
	const float zoom = ( float )pZoom * 0.01f;
	
	const int imageWidth = ( int )( ( float )engImage->GetWidth() * zoom );
	const int imageHeight = ( int )( ( float )engImage->GetHeight() * zoom );
	const int imageOffsetX = ( parentSize.x - imageWidth ) / 2 + pOffset.x;
	const int imageOffsetY = ( parentSize.y - imageHeight ) / 2 + pOffset.y;
	
	if( pCanvasFontImage ){
		pCanvasFontImage->SetImage( engImage );
		pCanvasFontImage->SetVisible( true );
		pCanvasFontImage->SetPosition( decPoint( imageOffsetX, imageOffsetY ) );
		pCanvasFontImage->SetSize( decPoint( imageWidth, imageHeight ) );
	}
	
	if( pCanvasImageBackground ){
		pCanvasImageBackground->SetPosition( pCanvasFontImage->GetPosition() );
		pCanvasImageBackground->SetSize( pCanvasFontImage->GetSize() );
		pCanvasImageBackground->SetVisible( true );
	}
	
	if( pCanvasActiveGlyph ){
		const feFontGlyph * const activeGlyph = pFont->GetGlyphSelection().GetActiveGlyph();
		
		if( activeGlyph ){
			pCanvasActiveGlyph->SetVisible( true );
			pCanvasActiveGlyph->SetThickness( zoom );
			
			const int glyphU = activeGlyph->GetU();
			const int glyphV = activeGlyph->GetV();
			const int glyphWidth = activeGlyph->GetWidth();
			const int glyphHeight = pFont->GetLineHeight();
			
			const int x1 = imageOffsetX + ( int )( ( float )glyphU * zoom );
			const int y1 = imageOffsetY + ( int )( ( float )glyphV * zoom );
			const int x2 = imageOffsetX + ( int )( ( float )( glyphU + glyphWidth ) * zoom );
			const int y2 = imageOffsetY + ( int )( ( float )( glyphV + glyphHeight ) * zoom );
			
			pCanvasActiveGlyph->SetPosition( decPoint( x1, y1 ) );
			pCanvasActiveGlyph->SetSize( decPoint( x2 - x1, y2 - y1 ) );
			
		}else{
			pCanvasActiveGlyph->SetVisible( false );
		}
	}
}
