/* 
 * Drag[en]gine IGDE
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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxFont.h"
#include "../../../resources/igdeFont.h"
#include "../../../../engine/igdeEngineController.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontBuilder.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/image/deImageReference.h>



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
	sGlyph *pGlyphs;
	int pGlyphCount;
	int pImageWidth;
	int pImageHeight;
	deImageReference pImage;
	
	
	
public:
	igdeFont_FontBuilder( igdeFont &font ) :
	pFont( font ),
	pFirstCode( 32 ),
	pLastCode( 255 ),
	pMinImageWidth( 64 ),
	pMaxImageWidth( 1024 ),
	pBorderSize( 0 ),
	pEnlargeGlyph( 1 ),
	pLineHeight( 0 ),
	pGlyphs( NULL ),
	pGlyphCount( 0 ),
	pImageWidth( 0 ),
	pImageHeight( 0 ){
	}
	
	virtual ~igdeFont_FontBuilder(){
		if( pGlyphs ){
			delete [] pGlyphs;
		}
	}
	
	
	
	deFont *GenerateFont(){
		const FXFont &nativeFont = *( ( FXFont* )pFont.GetNativeFont() );
		igdeEnvironment &environment = pFont.GetEnvironment();
		deLogger &logger = *environment.GetLogger();
		int ascent, descent, spacing;
		
		ascent = nativeFont.getFontAscent();
		descent = nativeFont.getFontDescent();
		spacing = nativeFont.getFontSpacing(); // not used yet in the font object
		pLineHeight = ascent + descent + pEnlargeGlyph * 2;
		logger.LogInfoFormat( "DEIGDE", "Generate font: ascent=%i descent=%i spacing=%i lineHeight=%i slant=%i actualSlant=%i",
			ascent, descent, spacing, pLineHeight, nativeFont.getSlant(), nativeFont.getActualSlant() );
		
		pAddGlyphs();
		pCalcLayout();
		pRenderImage();
		
		return pFont.GetEnvironment().GetEngineController()->GetEngine()->GetFontManager()->CreateFont( "", *this );
	}
	
	
	
	virtual void BuildFont( deFont *engFont ){
		if( ! engFont ){
			DETHROW( deeInvalidParam );
		}
		
		engFont->SetIsColorFont( false );
		engFont->SetLineHeight( pLineHeight );
		engFont->SetImage( pImage );
		engFont->SetGlyphCount( pGlyphCount );
		
		int i;
		for( i=0; i<pGlyphCount; i++ ){
			deFontGlyph &engGlyph = engFont->GetGlyphAt( i );
			const sGlyph &glyph = pGlyphs[ i ];
			
			engGlyph.SetUnicode( glyph.code );
			engGlyph.SetX( glyph.x );
			engGlyph.SetY( glyph.y );
			engGlyph.SetZ( 0 );
			engGlyph.SetWidth( glyph.width );
			engGlyph.SetBearing( glyph.bearing );
			engGlyph.SetAdvance( glyph.advance );
		}
	}
	
	
	
private:
	void pAddGlyphs(){
		if( pGlyphs ){
			DETHROW( deeInvalidParam );
		}
		
		FXFont * const nativeFont = ( FXFont* )pFont.GetNativeFont();
		const FXColor colorBg = FXRGB( 0, 0, 0 );
		const FXColor colorFg = FXRGB( 255, 255, 255 );
		const int ascent = nativeFont->getFontAscent();
		decUnicodeString character;
		FXColor *foxImageData = NULL;
		FXImage *foxImage = NULL;
		FXDCWindow *dc = NULL;
		int leftBearing;
		int rightBearing;
		int c, y;
		
		try{
			pGlyphs = new sGlyph[ 256 ];
			pGlyphCount = 0;
			
			// this hack here is required since the character measuring in fox is incomplete
			// or downright broken, to fix this problem a reverse approach is used. each
			// character is rendered into an empty image quite a bit larger along the horizontal
			// direction. the bearings are then found by looking for the first colon from both
			// edges where the pixel color is not the background color. this finds the exact
			// values no matter how the rendering is implemented in fox or the platform nor
			// however this can change in the future.
			const int imageHeight = pLineHeight;
			const int drawPoint = imageHeight;
			const int imageWidth = drawPoint * 3; // one line-height in each direction should be enough
			const int pixelCount = imageWidth * imageHeight;
			
			foxImageData = new FXColor[ imageWidth * imageHeight ];
			memset( foxImageData, 0, sizeof( FXColor ) * pixelCount );
			
			// create a fox image in memory with memory-only parameters since we do not intend
			// to use this image for rendering later on
			foxImage = new FXImage( FXApp::instance(), foxImageData, IMAGE_KEEP | IMAGE_NEAREST, imageWidth, imageHeight );
			foxImage->create();
			
			dc = new FXDCWindow( foxImage );
			dc->setFont( nativeFont );
			
			// process characters
			for( c=pFirstCode; c<=pLastCode; c++ ){
				if( ! nativeFont->hasChar( c ) ){
					continue;
				}
				
				// the width can not go lower than this
				const int minWidth = nativeFont->getCharWidth( c );
				
				// clear background and draw glyph
				dc->setForeground( colorBg );
				dc->fillRectangle( 0, 0, imageWidth, imageHeight );
				
				character.SetFromUTF8( "" );
				character.AppendCharacter( c );
				const decString utf8( character.ToUTF8() );
				dc->setForeground( colorFg );
				dc->setBackground( colorFg );
				dc->drawText( drawPoint, ascent, utf8.GetString(), utf8.GetLength() );
				
				foxImage->restore();
				
				// analyze where the glyph ends at the left and right side to find the bearings
				for( leftBearing=0; leftBearing<drawPoint; leftBearing++ ){
					for( y=0; y<imageHeight; y++ ){
						if( foxImageData[ imageWidth*y + leftBearing ] != colorBg ){
							break; // we found the left edge
						}
					}
					if( y < imageHeight ){
						break;
					}
				}
				
				for( rightBearing=imageWidth-1; rightBearing>drawPoint+minWidth; rightBearing-- ){
					for( y=0; y<imageHeight; y++ ){
						if( foxImageData[ imageWidth*y + rightBearing ] != colorBg ){
							break; // we found the right edge
						}
					}
					if( y < imageHeight ){
						break;
					}
				}
				//pEnvironment->GetLogger()->LogInfoFormat( LOGSOURCE, "Glyph %i(%c): dp=%i mw=%i lb=%i rb=%i", c, c, drawPoint, minWidth, leftBearing, rightBearing );
				
				// store the found values
				sGlyph &glyph = pGlyphs[ pGlyphCount++ ];
				glyph.code = c;
				glyph.x = 0;
				glyph.y = 0;
				glyph.width = rightBearing - leftBearing + 1 + pEnlargeGlyph * 2;
				glyph.bearing = drawPoint - leftBearing + pEnlargeGlyph;
				glyph.advance = minWidth;
			}
			
			// clean up
			delete dc;
			dc = NULL;
			
			delete foxImage;
			foxImage = NULL;
			
			delete [] foxImageData;
			
		}catch( const deException & ){
			if( dc ){
				delete dc;
			}
			if( foxImage ){
				delete foxImage;
			}
			if( foxImageData ){
				delete [] foxImageData;
			}
			throw;
		}
	}
	
	void pCalcLayout(){
		deLogger &logger = *pFont.GetEnvironment().GetLogger();
		int maxGlyphWidth;
		int g, u, v, width;
		int fullWidth;
		
		// determine the width of all the glyphs placed on one line
		fullWidth = pBorderSize;
		maxGlyphWidth = 0;
		
		for( g=0; g<pGlyphCount; g++ ){
			const sGlyph &glyph = pGlyphs[ g ];
			maxGlyphWidth = decMath::max( maxGlyphWidth, glyph.width );
			fullWidth += glyph.width + pBorderSize;
		}
		
		logger.LogInfoFormat( "DEIGDE", "Calculate layout: fullWidth=%i maxGlyphWidth=%i", fullWidth, maxGlyphWidth );
		maxGlyphWidth = ( int )( ( float )maxGlyphWidth * 0.75f );
		
		// determine the smallest image width where the glyph line can be
		// wrapped to so that the height does not exceed the image width
		// ( hence the glyphs would all fit into a square texture )
		pImageHeight = 0; // to stop compiler from complaining
		for( pImageWidth=pMinImageWidth; pImageWidth<=pMaxImageWidth; pImageWidth<<=1 ){
			pImageHeight = pLineHeight * ( ( fullWidth - maxGlyphWidth ) / pImageWidth + 1 );
			logger.LogInfoFormat( "DEIGDE", "Testing image size %ix%i", pImageWidth, pImageHeight );
			if( pImageHeight <= pImageWidth ) break;
		}
		logger.LogInfoFormat( "DEIGDE", "Using image size %ix%i", pImageWidth, pImageHeight );
		
		// layout the glyphs using the found image width
		u = pBorderSize;
		v = pBorderSize;
		
		for( g=0; g<pGlyphCount; g++ ){
			sGlyph &glyph = pGlyphs[ g ];
			
			width = glyph.width;
			
			if( u + width > pImageWidth ){
				u = pBorderSize;
				v += pLineHeight + pBorderSize;
			}
			
			glyph.x = u;
			glyph.y = v;
			
			u += width + pBorderSize;
		}
		
		// determine a matching image height for the generated layout
		pImageHeight = v + pLineHeight + pBorderSize;
		logger.LogInfoFormat( "DEIGDE", "Final image size %ix%i", pImageWidth, pImageHeight );
	}
	
	void pRenderImage(){
		FXFont * const nativeFont = ( FXFont* )pFont.GetNativeFont();
		FXColor *foxImageData = NULL;
		decUnicodeString character;
		FXImage *foxImage = NULL;
		FXDCWindow *dc = NULL;
		FXColor foxColor;
		int p, g, x, y;
		
		const int pixelCount = pImageWidth * pImageHeight;
		const int ascent = nativeFont->getFontAscent();
		
		try{
			pImage.TakeOver( pFont.GetEnvironment().GetEngineController()->GetEngine()
				->GetImageManager()->CreateImage( pImageWidth, pImageHeight, 1, 4, 8 ) );
			
			// create image data in memory cleared to black. we can not clear
			// to transparent since fox does not properly support images with
			// transparency. instead we render a grayscale image and are using
			// the grayscale value also as alpha value.
			foxImageData = new FXColor[ pImageWidth * pImageHeight ];
			for( p=0; p<pixelCount; p++ ){
				foxImageData[ p ] = FXRGB( 0, 0, 0 );
			}
			
			// create a fox image in memory with memory-only parameters since
			// we do not intend to use this image for rendering later on
			foxImage = new FXImage( FXApp::instance(), foxImageData, IMAGE_KEEP | IMAGE_NEAREST, pImageWidth, pImageHeight );
			foxImage->create();
			
			dc = new FXDCWindow( foxImage );
			
			// render all glyphs at their stored location using solid white
			dc->setForeground( FXRGB( 255, 255, 255 ) );
			dc->setFont( nativeFont );
			
			for( g=0; g<pGlyphCount; g++ ){
				const sGlyph &glyph = pGlyphs[ g ];
				
				x = glyph.x + glyph.bearing + pEnlargeGlyph;
				y = glyph.y + ascent + pEnlargeGlyph;
				
				character.SetFromUTF8( "" );
				character.AppendCharacter( glyph.code );
				const decString utf8 = character.ToUTF8();
				dc->drawText( x, y, utf8.GetString(), utf8.GetLength() );
			}
			
			foxImage->restore();
			
			// copy the fox image over to the font image
			if( pImage->GetBitCount() == 8 ){
				sRGBA8 *data = pImage->GetDataRGBA8();
				
				for( p=0; p<pixelCount; p++ ){
					sRGBA8 &pixel = data[ p ];
					foxColor = foxImageData[ p ];
					
					pixel.red = 255;
					pixel.green = 255;
					pixel.blue = 255;
					pixel.alpha = ( unsigned char )( FXREDVAL( foxColor ) );
				}
				
			}else if( pImage->GetBitCount() == 16 ){
				sRGBA16 *data = pImage->GetDataRGBA16();
				
				for( p=0; p<pixelCount; p++ ){
					sRGBA16 &pixel = data[ p ];
					foxColor = foxImageData[ p ];
					
					pixel.red = 65535;
					pixel.green = 65535;
					pixel.blue = 65535;
					pixel.alpha = ( unsigned short )( ( FXREDVAL( foxColor ) ) << 8 );
				}
				
			}else{ // engImage.GetBitCount() == 32
				sRGBA32 *data = pImage->GetDataRGBA32();
				float factor = 1.0f / 255.0f;
				
				for( p=0; p<pixelCount; p++ ){
					sRGBA32 &pixel = data[ p ];
					foxColor = foxImageData[ p ];
					
					pixel.red = 1.0f;
					pixel.green = 1.0f;
					pixel.blue = 1.0f;
					pixel.alpha = ( float )( FXREDVAL( foxColor ) ) * factor;
				}
			}
			
			// prepare image for use
			pImage->NotifyImageDataChanged();
			
			// clean up
			delete dc;
			dc = NULL;
			
			delete foxImage;
			foxImage = NULL;
			
			delete [] foxImageData;
			
		}catch( const deException & ){
			if( dc ) delete dc;
			if( foxImage ) delete foxImage;
			if( foxImageData ) delete [] foxImageData;
			throw;
		}
	}
};



// Events
///////////

FXDEFMAP( igdeNativeFoxFont ) igdeNativeFoxFontMap[] = {};



// Class igdeNativeFoxFont
////////////////////////////

FXIMPLEMENT( igdeNativeFoxFont, FXFont, igdeNativeFoxFontMap, ARRAYNUMBER( igdeNativeFoxFontMap ) )

// Constructor, destructor
////////////////////////////


igdeNativeFoxFont::igdeNativeFoxFont(){ }

igdeNativeFoxFont::igdeNativeFoxFont( igdeFont &powner ) :
// WARNING the documentation in fox is inconsistent. for both FXFontDesc and the FXFont
//         constructor the documentation states the size is in 1/10 points. for FXFontDesc
//         this is correct but FXFont constructor expects suddenly 1/1 points.
FXFont( FXApp::instance(), powner.GetName().GetString(), powner.GetSize(),
	powner.GetBold() ? FXFont::Bold : FXFont::Normal,
	powner.GetItalic() ? FXFont::Italic : FXFont::Straight ),
pOwner( &powner ){
}

igdeNativeFoxFont::~igdeNativeFoxFont(){
}

igdeNativeFoxFont *igdeNativeFoxFont::CreateNativeFont( igdeFont &powner ){
	igdeNativeFoxFont * const native = new igdeNativeFoxFont( powner );
	native->create();
	return native;
}

void igdeNativeFoxFont::DestroyNativeFont(){
	delete this;
}



// Management
///////////////

deFont *igdeNativeFoxFont::CreateEngineFont(){
	return igdeFont_FontBuilder( *pOwner ).GenerateFont();
}

decPoint igdeNativeFoxFont::TextSize( const char *text ) const{
	const int len = strlen( text );
	return decPoint( getTextWidth( text, len ), getTextHeight( text, len ) );
}

#endif
