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

#include "feLoadSaveFont.h"
#include "../font/feFont.h"
#include "../font/glyph/feFontGlyph.h"
#include "../font/image/feFontImage.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/font/deFontBuilder.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/systems/modules/font/deBaseFontModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class feLoadSaveFont
/////////////////////////

// Constructor, destructor
////////////////////////////

feLoadSaveFont::feLoadSaveFont( deBaseFontModule *module ) :
pModule( module )
{
	if( ! module ){
		DETHROW( deeInvalidParam );
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	for( i=0; i<patternCount; i++ ){
		if( i > 0 ){
			pPattern.AppendCharacter( ',' );
		}
		pPattern.AppendCharacter( '*' );
		pPattern.Append( patternList.GetAt( i ) );
	}
}

feLoadSaveFont::~feLoadSaveFont(){
	pCleanUp();
}



// Management
///////////////

void feLoadSaveFont::SetName( const char *name ){
	pName = name;
}

void feLoadSaveFont::SetPattern( const char *pattern ){
	pPattern = pattern;
}



// Loading and saving
///////////////////////

class cDirectFontLoader : public deFontBuilder{
private:
	deBaseFontModule *pModule;
	decBaseFileReader *pFile;
	decString pVirtualPath;
	deEngine *pEngine;
	
public:
	cDirectFontLoader( deEngine *engine, const char *virtualPath, decBaseFileReader *file, deBaseFontModule *module ){
		pModule = module;
		pFile = file;
		pEngine = engine;
		pVirtualPath = virtualPath;
	}
	virtual ~cDirectFontLoader(){}
	
	virtual void BuildFont( deFont *font ){
		deImageManager &imageMgr = *pEngine->GetImageManager();
		deImage *image = NULL;
		decPath basePath;
		
		pModule->LoadFont( *pFile, *font );
		
		try{
			basePath.SetFromUnix( pVirtualPath.GetString() );
			basePath.RemoveLastComponent();
			
			if( strlen( font->GetImagePath() ) > 0 ){
				image = imageMgr.LoadImage( font->GetImagePath(), basePath.GetPathUnix() );
				font->SetImage( image );
				image->FreeReference();
				image = NULL;
			}
			
		}catch( const deException & ){
			if( image ) image->FreeReference();
			throw;
		}
	}
};

void feLoadSaveFont::LoadFont( const char *virtualPath, feFont *font, decBaseFileReader *file ){
	if( ! font || ! file ) DETHROW( deeInvalidParam );
	
	feFontImage &fontImage = *font->GetFontImage();
	deEngine *engine = pModule->GetGameEngine();
	feFontGlyph *glyph = NULL;
	deFont *engFont = NULL;
	
	cDirectFontLoader directFontLoader( engine, virtualPath, file, pModule );
	
	// protect this area
	try{
		// try to load the font
		engFont = engine->GetFontManager()->CreateFont( "", directFontLoader );
		
		// store font information
		font->SetFilePath( virtualPath );
		font->SetLineHeight( engFont->GetLineHeight() );
		font->SetColorFont( engFont->GetIsColorFont() );
		
		// store font image information
		fontImage.SetFilename( engFont->GetImagePath(), true );
		
		// store glyph information
		const int glyphCount = engFont->GetGlyphCount();
		int i;
		
		// TODO engFont->GetUndefinedGlyph()
		
		for( i=0; i<glyphCount; i++ ){
			const deFontGlyph &engGlyph = engFont->GetGlyphAt( i );
			
			glyph = new feFontGlyph;
			glyph->SetCode( engGlyph.GetUnicode() );
			glyph->SetU( engGlyph.GetX() );
			glyph->SetV( engGlyph.GetY() );
			// engGlyph.GetZ()
			glyph->SetWidth( engGlyph.GetWidth() );
			glyph->SetBearing( engGlyph.GetBearing() );
			glyph->SetAdvance( engGlyph.GetAdvance() );
			
			font->AddGlyph( glyph );
			glyph = NULL;
		}
		
		// time to release the font resource
		engFont->FreeReference();
		
	}catch( const deException &e ){
		e.PrintError();
		if( glyph ){
			glyph->FreeReference();
		}
		if( engFont ){
			engFont->FreeReference();
		}
		throw;
	}
}

void feLoadSaveFont::SaveFont( const char *virtualPath, feFont *font, decBaseFileWriter *file ){
	if( ! font || ! file ) DETHROW( deeInvalidParam );
	
	deEngine &engine = *pModule->GetGameEngine();
	deImageManager &imgmgr = *engine.GetImageManager();
	
	// this is now another little hack. usually we would have to build a temporary
	// font to save but the font is already existing if nothing is wrong. we rebuild
	// the font if not done already
	font->Rebuild();
	
	// if nothing went wrong we have now a font to save
	if( ! font->GetEngineFont() ) DETHROW( deeInvalidParam );
	
	pModule->SaveFont( *file, *font->GetEngineFont() );
	font->SetFilePath( virtualPath );
	
	// save the fonr image if required
	feFontImage &fontImage = *font->GetFontImage();
	
	if( fontImage.GetChanged() && fontImage.GetEngineImage() ){
		decPath path;
		
		path.SetFromUnix( fontImage.GetFilename() );
		if( path.IsRelative() ){
			path.SetFromUnix( font->GetDirectoryPath() );
			path.AddUnixPath( fontImage.GetFilename() );
		}
		
		imgmgr.SaveImage( fontImage.GetEngineImage(), path.GetPathUnix() );
		
		fontImage.SetSaved( true );
		fontImage.SetChanged( false );
	}
	
}



// Private functions
//////////////////////

void feLoadSaveFont::pCleanUp(){
}
