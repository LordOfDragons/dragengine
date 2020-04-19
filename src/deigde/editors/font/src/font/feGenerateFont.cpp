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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feFont.h"
#include "feGenerateFont.h"
#include "glyph/feFontGlyph.h"
#include "image/feFontImage.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/resources/igdeFontReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>



// Definitions
////////////////

#define LOGSOURCE "Font Editor"



// Class feFont
/////////////////

// Constructor, destructor
////////////////////////////

feGenerateFont::feGenerateFont( igdeEnvironment &environment ) :
pEnvironment( environment ),
pFirstCode( 32 ),
pLastCode( 255 ),
pMinImageWidth( 64 ),
pMaxImageWidth( 1024 ),
pBorderSize( 1 ),
pEnlargeGlyph( 0 ){
}

feGenerateFont::~feGenerateFont(){
}



// Management
///////////////

void feGenerateFont::SetFontConfig( const igdeFont::sConfiguration &config ){
	pFontConfig = config;
}

void feGenerateFont::SetCodeRange( int first, int last ){
	if( first < 0 || last < 0 || first > last ){
		DETHROW( deeInvalidParam );
	}
	
	pFirstCode = first;
	pLastCode = last;
}

void feGenerateFont::SetBorderSize( int borderSize ){
	if( borderSize < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pBorderSize = borderSize;
}

void feGenerateFont::SetEnlargeGlpyh( int enlarge ){
	if( enlarge < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pEnlargeGlyph = enlarge;
}



feFont* feGenerateFont::GenerateFont(){
	pSystemFont = pEnvironment.GetSharedFont( pFontConfig )->GetEngineFont();
	
	deObjectReference fontRef;
	fontRef.TakeOver( new feFont( &pEnvironment ) );
	feFont * const font = ( feFont* )( deObject* )fontRef;
	
	font->SetLineHeight( pSystemFont->GetLineHeight() + pEnlargeGlyph * 2  );
	
	pAddGlyphs( *font );
	pCalcLayout( *font );
	pRenderImage( *font );
	
	font->AddReference(); // caller takes over reference
	return font;
}



// Private Functions
//////////////////////

void feGenerateFont::pAddGlyphs( feFont &font ){
	deObjectReference glyphRef;
	int i;
	
	for( i=pFirstCode; i<=pLastCode; i++ ){
		if( ! pSystemFont->HasGlyph( i ) ){
			continue;
		}
		
		const deFontGlyph &sfGlyph = pSystemFont->GetGlyph( i );
		
		glyphRef.TakeOver( new feFontGlyph );
		feFontGlyph * const glyph = ( feFontGlyph* )( deObject* )glyphRef;
		glyph->SetCode( i );
		glyph->SetU( 0 );
		glyph->SetV( 0 );
		glyph->SetWidth( sfGlyph.GetWidth() + pEnlargeGlyph * 2 );
		glyph->SetBearing( sfGlyph.GetBearing() + pEnlargeGlyph );
		glyph->SetAdvance( glyph->GetWidth() );
		
		font.AddGlyph( glyph );
	}
}

void feGenerateFont::pCalcLayout( feFont &font ){
	const int lineHeight = font.GetLineHeight();
	deLogger &logger = *pEnvironment.GetLogger();
	int g, glyphCount = font.GetGlyphCount();
	int imageHeight, imageWidth;
	
	// determine the width of all the glyphs placed on one line
	int fullWidth = pBorderSize;
	int maxGlyphWidth = 0;
	
	for( g=0; g<glyphCount; g++ ){
		const feFontGlyph &glyph = *font.GetGlyphAt( g );
		maxGlyphWidth = decMath::max( maxGlyphWidth, glyph.GetWidth() );
		fullWidth += glyph.GetWidth() + pBorderSize;
	}
	
	logger.LogInfoFormat( LOGSOURCE, "Calculate layout: fullWidth=%i maxGlyphWidth=%i", fullWidth, maxGlyphWidth );
// 	maxGlyphWidth = ( int )( ( float )maxGlyphWidth * 0.75f );
	
	// determine the smallest image width where the glyph line can be
	// wrapped to so that the height does not exceed the image width
	// ( hence the glyphs would all fit into a square texture )
	imageHeight = 0; // to stop compiler from complaining
	for( imageWidth=pMinImageWidth; imageWidth<=pMaxImageWidth; imageWidth<<=1 ){
		imageHeight = lineHeight * ( ( fullWidth - maxGlyphWidth ) / imageWidth + 1 );
		logger.LogInfoFormat( LOGSOURCE, "Testing image size %ix%i", imageWidth, imageHeight );
		if( imageHeight <= imageWidth ) break;
	}
	logger.LogInfoFormat( LOGSOURCE, "Using image size %ix%i", imageWidth, imageHeight );
	
	// layout the glyphs using the found image width
	int u = pBorderSize;
	int v = pBorderSize;
	
	for( g=0; g<glyphCount; g++ ){
		feFontGlyph &glyph = *font.GetGlyphAt( g );
		
		const int width = glyph.GetWidth();
		
		if( u + width > imageWidth ){
			u = pBorderSize;
			v += lineHeight + pBorderSize;
		}
		
		glyph.SetU( u );
		glyph.SetV( v );
		
		u += width + pBorderSize;
	}
	
	// determine a matching image height for the generated layout
	imageHeight = v + lineHeight + pBorderSize;
	logger.LogInfoFormat( LOGSOURCE, "Final image size %ix%i", imageWidth, imageHeight );
	
	// set the image size which also creates the image to draw in the next step
	font.GetFontImage()->SetSize( imageWidth, imageHeight );
}

void feGenerateFont::pRenderImage( feFont &font ){
	const int sysFontLineHeight = pSystemFont->GetLineHeight();
	const deImage &sysFontImage = *pSystemFont->GetImage();
	feFontImage &fontImage = *font.GetFontImage();
	deImage &engImage = *fontImage.GetEngineImage();
	const int glyphCount = font.GetGlyphCount();
	const int imageHeight = engImage.GetHeight();
	const int imageWidth = engImage.GetWidth();
	const int pixelCount = imageWidth * imageHeight;
	decUnicodeString character;
	int i, dx, dy, sx, sy;
	
	// clear image
	const sRGBA8 * const sysFontImageData = sysFontImage.GetDataRGBA8();
	sRGBA8 * const imageData = engImage.GetDataRGBA8();
	
	for( i=0; i<pixelCount; i++ ){
		imageData[ i ].red = 0;
		imageData[ i ].green = 0;
		imageData[ i ].blue = 0;
		imageData[ i ].alpha = 0;
	}
	
	// copy glyphs at their stored location using solid white
	for( i=0; i<glyphCount; i++ ){
		const feFontGlyph &glyph = *font.GetGlyphAt( i );
		const int left = glyph.GetU() + pEnlargeGlyph;
		const int top = glyph.GetV() + pEnlargeGlyph;
		
		const deFontGlyph &sfGlyph = pSystemFont->GetGlyph( glyph.GetCode() );
		const int srcLeft = sfGlyph.GetX();
		const int srcRight = srcLeft + sfGlyph.GetWidth();
		const int srcTop = sfGlyph.GetY();
		const int srcBottom = srcTop + sysFontLineHeight;
		
		for( dy=top, sy=srcTop; sy<srcBottom; dy++, sy++ ){
			const sRGBA8 * const sysFontImageDataLine = sysFontImageData + sysFontImage.GetWidth() * sy;
			sRGBA8 * const imageDataLine = imageData + imageWidth * dy;
			
			for( dx=left, sx=srcLeft; sx<srcRight; dx++, sx++ ){
				imageDataLine[ dx ] = sysFontImageDataLine[ sx ];
			}
		}
	}
	
	// prepare image for use
	engImage.NotifyImageDataChanged();
}
