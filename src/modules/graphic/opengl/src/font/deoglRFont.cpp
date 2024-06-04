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
#include <stdlib.h>
#include <string.h>

#include "deoglRFont.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../texture/deoglImage.h"
#include "../texture/deoglRImage.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglRFont
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRFont::deoglRFont( deoglRenderThread &renderThread, const deFont &font ) :
pRenderThread( renderThread ),

pGlyphs( NULL ),
pLineHeight( font.GetLineHeight() ),
pIsColorFont( font.GetIsColorFont() ),

pDelayedImage( NULL )
{
	try{
		pBuildGlyphs( font );
		
		// register for delayed async res initialize and delayed opengl object creation
		pRenderThread.GetDelayedOperations().AddAsyncResInitFont( this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, Font );
}

deoglRFont::~deoglRFont(){
	LEAK_CHECK_FREE( pRenderThread, Font );
	pCleanUp();
}



// Management
///////////////

void deoglRFont::FinalizeAsyncResLoading(){
	// FinalizeAsyncResLoading is called from main thread
	if( ! pDelayedImage ){
		return;
	}
	
	// ensure image texture is created. this required SyncToRender() called from the main
	// thread and PrepareForRender() called in the render thread using delayed operations.
	pDelayedImage->SyncToRender();
	
	pImage = pDelayedImage->GetRImage();
	pRenderThread.GetDelayedOperations().AddInitImage( pImage );
}



// Private Functions
//////////////////////

void deoglRFont::pCleanUp(){
	pRenderThread.GetDelayedOperations().RemoveAsyncResInitFont( this );
	
	if( pImage ){
		pRenderThread.GetDelayedOperations().RemoveInitImage( pImage );
	}
	if( pGlyphs ){
		delete [] pGlyphs;
	}
}

void deoglRFont::pBuildGlyphs( const deFont &font ){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	// TODO change this to properly use font glyphs. right now the codes 0-255 are allowed
	//      and filled as a fixed size array
	pGlyphs = new sGlyph[ 256 ];
	
	deImage * const image = font.GetImage();
	
	if( image ){
		pDelayedImage = ( deoglImage* )image->GetPeerGraphic();
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	pSetGlyph( pUndefinedGlyph, font.GetUndefinedGlyph() );
	
	int i;
	for( i=0; i<256; i++ ){
		pGlyphs[ i ] = pUndefinedGlyph;
	}
	
	const int glyphCount = font.GetGlyphCount();
	for( i=0; i<glyphCount; i++ ){
		const deFontGlyph &glyph = font.GetGlyphAt( i );
		if( glyph.GetUnicode() < 256 ){
			pSetGlyph( pGlyphs[ glyph.GetUnicode() ], glyph );
		}
	}
}

void deoglRFont::pSetGlyph( sGlyph &target, const deFontGlyph &source ){
	target.x1 = ( float )source.GetX();
	target.y1 = ( float )source.GetY();
	target.x2 = ( float )( source.GetX() + source.GetWidth() );
	target.y2 = ( float )( source.GetY() + pLineHeight );
	// target.z = source.GetZ()
	target.bearing = source.GetBearing();
	target.width = source.GetWidth();
	target.height = pLineHeight;
	target.advance = source.GetAdvance();
}
