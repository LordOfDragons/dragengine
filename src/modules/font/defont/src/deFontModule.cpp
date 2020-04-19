/* 
 * Drag[en]gine Font Module
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

#include "deFontModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>



// Export definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEFontCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry function
///////////////////

deBaseModule *DEFontCreateModule( deLoadableModule *loadableModule ) {
	deBaseModule *module = NULL;
	
	try {
		module = new deFontModule( *loadableModule );
		
	} catch ( const deException & ) {
	}
	
	return module;
}



// Class deFontModule
///////////////////////

// Constructor, destructor
////////////////////////////

deFontModule::deFontModule( deLoadableModule &loadableModule ) :
deBaseFontModule( loadableModule ) {
}

deFontModule::~deFontModule() {
}



// Loading and saving
///////////////////////

void deFontModule::LoadFont( decBaseFileReader &file, deFont &font ) {
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetGameEngine()->GetLogger() ).ParseXml( &file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if ( ! root || strcmp( root->GetName(), "font" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pParseFont( root, font );
}

void deFontModule::SaveFont( decBaseFileWriter &file, const deFont &font ) {
	decXmlWriter writer( &file );
	writer.WriteXMLDeclaration();
	pWriteFont( writer, font );
}



// Private functions
//////////////////////

const char *deFontModule::pGetAttributeString( decXmlElementTag *tag, const char *name ) {
	decXmlAttValue *value = tag->FindAttribute( name );
	if ( value ) {
		return value->GetValue();
	} else {
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

bool deFontModule::pHasAttributeString( decXmlElementTag *tag, const char *name ) {
	return tag->FindAttribute( name ) != NULL;
}

int deFontModule::pGetAttributeInt( decXmlElementTag *tag, const char *name ) {
	decXmlAttValue *value = tag->FindAttribute( name );
	if ( value ) {
		return ( int )strtol( value->GetValue(), NULL, 10 );
	} else {
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}
float deFontModule::pGetAttributeFloat( decXmlElementTag *tag, const char *name ) {
	decXmlAttValue *value = tag->FindAttribute( name );
	if ( value ) {
		return strtof( value->GetValue(), NULL );
	} else {
		LogErrorFormat( "Missing Attribute '%s' in tag '%s'\n", name, tag->GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

void deFontModule::pParseFont( decXmlElementTag *root, deFont &font ){
	decXmlElementTag *tag;
	int i;
	
	int glyphCount = 0;
	for( i=0; i<root->GetElementCount(); i++ ){
		tag = root->GetElementIfTag( i );
		if( tag && strcmp( tag->GetName(), "glyph" ) == 0 ){
			glyphCount++;
		}
	}
	
	font.SetGlyphCount( glyphCount );
	int glyphIndex = 0;
	
	for( i=0; i<root->GetElementCount(); i++ ){
		tag = root->GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "image" ) == 0 ){
			font.SetImagePath( pGetAttributeString( tag, "src" ) );
			
		}else if ( strcmp( tag->GetName(), "colorFont" ) == 0 ){
			font.SetIsColorFont( true );
			
		}else if ( strcmp( tag->GetName(), "lineHeight" ) == 0 ){
			font.SetLineHeight( ( int )strtol( tag->GetFirstData()->GetData(), NULL, 10 ) );
			
		}else if ( strcmp( tag->GetName(), "defaults" ) == 0 ){
			deFontGlyph &glyph = font.GetUndefinedGlyph();
			
			if( pHasAttributeString( tag, "u" ) ){
				// DEPRECATED old version
				glyph.SetX( pGetAttributeInt( tag, "u" ) );
				glyph.SetY( pGetAttributeInt( tag, "v" ) );
				glyph.SetWidth( pGetAttributeInt( tag, "width" ) );
				glyph.SetAdvance( glyph.GetWidth() );
				
			}else{
				glyph.SetX( pGetAttributeInt( tag, "x" ) );
				glyph.SetY( pGetAttributeInt( tag, "y" ) );
				glyph.SetZ( pGetAttributeInt( tag, "z" ) );
				glyph.SetWidth( pGetAttributeInt( tag, "width" ) );
				glyph.SetBearing( pGetAttributeInt( tag, "bearing" ) );
				glyph.SetAdvance( pGetAttributeInt( tag, "advance" ) );
			}
			
		} else if ( strcmp( tag->GetName(), "glyph" ) == 0 ) {
			deFontGlyph &glyph = font.GetGlyphAt( glyphIndex++ );
			
			if( pHasAttributeString( tag, "code" ) ){
				// DEPRECATED old version
				glyph.SetUnicode( pGetAttributeInt( tag, "code" ) );
				glyph.SetX( pGetAttributeInt( tag, "u" ) );
				glyph.SetY( pGetAttributeInt( tag, "v" ) );
				
			}else{
				glyph.SetUnicode( pGetAttributeInt( tag, "unicode" ) );
				glyph.SetX( pGetAttributeInt( tag, "x" ) );
				glyph.SetY( pGetAttributeInt( tag, "y" ) );
				glyph.SetZ( pGetAttributeInt( tag, "z" ) );
			}
			
			glyph.SetWidth( pGetAttributeInt( tag, "width" ) );
			glyph.SetBearing( pGetAttributeInt( tag, "bearing" ) );
			glyph.SetAdvance( pGetAttributeInt( tag, "advance" ) );
		}
	}
}



void deFontModule::pWriteFont( decXmlWriter& writer, const deFont& font ) {
	writer.WriteOpeningTag( "font", false, true );
	
	if ( strlen( font.GetImagePath() ) > 0 ) {
		writer.WriteOpeningTagStart( "image" );
		writer.WriteAttributeString( "src", font.GetImagePath() );
		writer.WriteOpeningTagEnd( true );
	}
	
	if ( font.GetIsColorFont() ) {
		writer.WriteDataTagString( "colorFont", "true" ); // hack, only presence of tag checked
	}
	
	writer.WriteDataTagInt( "lineHeight", font.GetLineHeight() );
	
	const deFontGlyph &undefinedGlyph = font.GetUndefinedGlyph();
	writer.WriteOpeningTagStart( "defaults" );
	writer.WriteAttributeInt( "x", undefinedGlyph.GetX() );
	writer.WriteAttributeInt( "y", undefinedGlyph.GetY() );
	writer.WriteAttributeInt( "z", undefinedGlyph.GetZ() );
	writer.WriteAttributeInt( "width", undefinedGlyph.GetWidth() );
	writer.WriteAttributeInt( "bearing", undefinedGlyph.GetBearing() );
	writer.WriteAttributeInt( "advance", undefinedGlyph.GetAdvance() );
	writer.WriteOpeningTagEnd( true );
	
	writer.WriteNewline();
	const int glyphCount = font.GetGlyphCount();
	int i;
	
	for ( i=0; i<glyphCount; i++ ) {
		const deFontGlyph &glyph = font.GetGlyphAt( i );
		
		writer.WriteOpeningTagStart( "glyph" );
		writer.WriteAttributeInt( "unicode", glyph.GetUnicode() );
		writer.WriteAttributeInt( "x", glyph.GetX() );
		writer.WriteAttributeInt( "y", glyph.GetY() );
		writer.WriteAttributeInt( "z", glyph.GetZ() );
		writer.WriteAttributeInt( "width", glyph.GetWidth() );
		writer.WriteAttributeInt( "bearing", glyph.GetBearing() );
		writer.WriteAttributeInt( "advance", glyph.GetAdvance() );
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteClosingTag( "font", true );
}
