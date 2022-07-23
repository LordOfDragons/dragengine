/* 
 * Drag[en]gine Game Engine
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
#include "decXmlWriter.h"
#include "../file/decBaseFileWriter.h"
#include "../string/unicode/decUnicodeString.h"
#include "../string/decString.h"
#include "../exceptions.h"
#include "../../dragengine_configuration.h"



// Macros
///////////

#define IS(what,value)			( (what) == (value) )
#define IN(what,lower,upper)	( (what) >= (lower) && (what) <= (upper) )



// Class decXmlWriter
///////////////////////

// Constructor, destructor
////////////////////////////

decXmlWriter::decXmlWriter( decBaseFileWriter *file ) :
pFile( NULL ),
pIndent( 0 )
{
	if( ! file ){
		DETHROW( deeInvalidParam );
	}
	
	pFile = file;
}

decXmlWriter::~decXmlWriter(){
}



// Management
///////////////

void decXmlWriter::SetIndent( int indent ){
	if( indent < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pIndent = indent;
}

void decXmlWriter::IncreaseIndent(){
	pIndent++;
}

void decXmlWriter::DecreaseIndent(){
	if( pIndent > 0 ){
		pIndent--;
	}
}



void decXmlWriter::WriteIndent(){
	int i;
	
	for( i=0; i<pIndent; i++ ){
		pFile->WriteByte( '\t' );
	}
}

void decXmlWriter::WriteUnicodeString( const decUnicodeString &string ){
	WriteTextString( string.ToUTF8() );
}

void decXmlWriter::WriteNewline(){
	WriteIndent();
	pFile->WriteByte( '\n' );
}



void decXmlWriter::WriteXMLDeclaration(){
	pFile->WriteString( "<?xml version='1.0' encoding='UTF-8'?>\n" );
}

void decXmlWriter::WriteDocTypeDeclaration( const char *rootTag, const char *dtdSource ){
	if( ! rootTag || ! dtdSource ){
		DETHROW( deeInvalidParam );
	}
	
	pFile->WriteString( "<!DOCTYPE " );
	pFile->WriteString( rootTag );
	pFile->WriteString( " SYSTEM '" );
	pFile->WriteString( dtdSource );
	pFile->WriteString( "'>\n" );
}

void decXmlWriter::WriteDocTypeDeclaration( const decUnicodeString &rootTag,
const decUnicodeString &dtdSource ){
	pFile->WriteString( "<!DOCTYPE " );
	WriteUnicodeString( rootTag );
	pFile->WriteString( " SYSTEM '" );
	WriteUnicodeString( dtdSource );
	pFile->WriteString( "'>\n" );
}

void decXmlWriter::WriteOpeningTag( const char *name, bool empty, bool indent, bool newline ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( indent ){
		WriteIndent();
	}
	
	pFile->WriteByte( '<' );
	pFile->WriteString( name );
	
	if( empty ){
		if( newline ){
			pFile->WriteString( "/>\n" );
			
		}else{
			pFile->WriteString( "/>" );
		}
		
	}else{
		if( newline ){
			pFile->WriteString( ">\n" );
			
		}else{
			pFile->WriteByte( '>' );
		}
		IncreaseIndent();
	}
}

void decXmlWriter::WriteOpeningTag( const decUnicodeString &name, bool empty,
bool indent, bool newline ){
	const decString utf8Name( name.ToUTF8() );
	WriteOpeningTag( utf8Name.GetString(), empty, indent, newline );
}

void decXmlWriter::WriteOpeningTagStart( const char *name, bool indent ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( indent ){
		WriteIndent();
	}
	
	pFile->WriteByte( '<' );
	pFile->WriteString( name );
}

void decXmlWriter::WriteOpeningTagStart( const decUnicodeString &name, bool indent ){
	const decString utf8Name( name.ToUTF8() );
	WriteOpeningTagStart( utf8Name.GetString(), indent );
}

void decXmlWriter::WriteAttributeString( const char *name, const char *string ){
	if( ! name || ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const char *walker = string;
	char buffer[ 10 ];
	
	pFile->WriteByte( ' ' );
	pFile->WriteString( name );
	pFile->WriteString( "='" );
	
	while( *walker ){
		if( *walker == '<' || *walker == '&' || *walker == '\'' ){
			#ifdef OS_W32_VS
				sprintf_s( buffer, 10, "&#%i;", *walker );
			#else
				sprintf( buffer, "&#%i;", *walker );
			#endif
			pFile->WriteString( buffer );
			
		}else{
			pFile->WriteByte( *walker );
		}
		
		walker++;
	}
	
	pFile->WriteByte( '\'' );
}

void decXmlWriter::WriteAttributeString( const char *name, const decString &string ){
	WriteAttributeString( name, string.GetString() );
}

void decXmlWriter::WriteAttributeString( const decUnicodeString &name,
const decUnicodeString &string ){
	const decString utf8Name( name.ToUTF8() );
	WriteAttributeString( utf8Name.GetString(), string.ToUTF8() );
}

void decXmlWriter::WriteAttributeBool( const char *name, bool value ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pFile->WriteByte( ' ' );
	pFile->WriteString( name );
	if( value ){
		pFile->WriteString( "='true'" );
		
	}else{
		pFile->WriteString( "='false'" );
	}
}

void decXmlWriter::WriteAttributeBool( const decUnicodeString &name, bool value ){
	const decString utf8Name( name.ToUTF8() );
	WriteAttributeBool( utf8Name.GetString(), value );
}

void decXmlWriter::WriteAttributeInt( const char *name, int value ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%i", value );
	#else
		sprintf( buffer, "%i", value );
	#endif
	
	pFile->WriteByte( ' ' );
	pFile->WriteString( name );
	pFile->WriteString( "='" );
	pFile->WriteString( buffer );
	pFile->WriteByte( '\'' );
}

void decXmlWriter::WriteAttributeInt( const decUnicodeString &name, int value ){
	const decString utf8Name( name.ToUTF8() );
	WriteAttributeInt( utf8Name.GetString(), value );
}

void decXmlWriter::WriteAttributeFloat( const char *name, float value ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%g", value );
	#else
		sprintf( buffer, "%g", value );
	#endif
	
	pFile->WriteByte( ' ' );
	pFile->WriteString( name );
	pFile->WriteString( "='" );
	pFile->WriteString( buffer );
	pFile->WriteByte( '\'' );
}

void decXmlWriter::WriteAttributeFloat( const decUnicodeString &name, float value ){
	const decString utf8Name( name.ToUTF8() );
	WriteAttributeFloat( utf8Name.GetString(), value );
}

void decXmlWriter::WriteAttributeDouble( const char *name, double value ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%g", value );
	#else
		sprintf( buffer, "%g", value );
	#endif
	
	pFile->WriteByte( ' ' );
	pFile->WriteString( name );
	pFile->WriteString( "='" );
	pFile->WriteString( buffer );
	pFile->WriteByte( '\'' );
}

void decXmlWriter::WriteAttributeDouble( const decUnicodeString &name, double value ){
	const decString utf8Name( name.ToUTF8() );
	WriteAttributeDouble( utf8Name.GetString(), value );
}

void decXmlWriter::WriteOpeningTagEnd( bool empty, bool newline ){
	if( empty ){
		if( newline ){
			pFile->WriteString( "/>\n" );
			
		}else{
			pFile->WriteString( "/>" );
		}
		
	}else{
		if( newline ){
			pFile->WriteString( ">\n" );
			
		}else{
			pFile->WriteByte( '>' );
		}
		IncreaseIndent();
	}
}

void decXmlWriter::WriteTextString( const char *text ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	
	const unsigned char *walker = ( const unsigned char * )text;
	char buffer[ 10 ];
	
	while( *walker ){
		/*
		if( IS( *walker, 0x9 ) ||  IS( *walker, 0xA ) ||  IS( *walker, 0xD )
		||  ( *walker >= 0x20 && ! ( IN( *walker, 0x7f, 0x84 ) || IN( *walker, 0x86, 0x9f ) ) ) ){
		*/
		
		if( *walker == '<' || *walker == '&' || *walker == '>' ){
			#ifdef OS_W32_VS
				sprintf_s( buffer, 10, "&#%i;", *walker );
			#else
				sprintf( buffer, "&#%i;", *walker );
			#endif
			pFile->WriteString( buffer );
			
		}else{
			pFile->WriteByte( *walker );
		}
		
		walker++;
	}
}

void decXmlWriter::WriteTextString( const decString &text ){
	WriteTextString( text.GetString() );
}

void decXmlWriter::WriteTextString( const decUnicodeString &text ){
	WriteTextString( text.ToUTF8() );
}

void decXmlWriter::WriteTextBool( bool value ){
	if( value ){
		pFile->WriteString( "true" );
		
	}else{
		pFile->WriteString( "false" );
	}
}

void decXmlWriter::WriteTextInt( int value ){
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%i", value );
	#else
		sprintf( buffer, "%i", value );
	#endif
	
	pFile->WriteString( buffer );
}

void decXmlWriter::WriteTextFloat( float value ){
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%g", value );
	#else
		sprintf( buffer, "%g", value );
	#endif
	
	pFile->WriteString( buffer );
}

void decXmlWriter::WriteTextDouble( double value ){
	char buffer[ 20 ];
	#ifdef OS_W32_VS
		sprintf_s( buffer, 20, "%g", value );
	#else
		sprintf( buffer, "%g", value );
	#endif
	
	pFile->WriteString( buffer );
}

void decXmlWriter::WriteClosingTag( const char *name, bool indent, bool newline ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	DecreaseIndent();
	if( indent ){
		WriteIndent();
	}
	
	pFile->WriteString( "</" );
	pFile->WriteString( name );
	
	if( newline ){
		pFile->WriteString( ">\n" );
		
	}else{
		pFile->WriteByte( '>' );
	}
}

void decXmlWriter::WriteClosingTag( const decUnicodeString &name, bool indent, bool newline ){
	const decString utf8Name( name.ToUTF8() );
	WriteClosingTag( utf8Name.GetString(), indent, newline );
}

void decXmlWriter::WriteComment( const char *comment, bool indent, bool newline ){
	if( ! comment ){
		DETHROW( deeInvalidParam );
	}
	
	if( indent ){
		WriteIndent();
	}
	
	pFile->WriteString( "<!-- " );
	pFile->WriteString( comment );
	
	if( newline ){
		pFile->WriteString( " -->\n" );
		
	}else{
		pFile->WriteString( " -->" );
	}
}

void decXmlWriter::WriteComment( const decString &comment, bool indent, bool newline ){
	WriteComment( comment.GetString(), indent, newline );
}

void decXmlWriter::WriteComment( const decUnicodeString &comment, bool indent, bool newline ){
	WriteComment( comment.ToUTF8(), indent, newline );
}



void decXmlWriter::WriteDataTagString( const char *tagName, const char *string,
bool indent, bool newline ){
	if( ! string ){
		DETHROW( deeInvalidParam );
	}
	
	if( string[ 0 ] ){
		WriteOpeningTag( tagName, false, indent, false );
		WriteTextString( string );
		WriteClosingTag( tagName, false, newline );
		
	}else{
		WriteOpeningTag( tagName, true, indent, true );
	}
}

void decXmlWriter::WriteDataTagString( const char *tagName, const decString &string,
bool indent, bool newline ){
	WriteDataTagString( tagName, string.GetString(), indent, newline );
}

void decXmlWriter::WriteDataTagString( const decUnicodeString &tagName,
const decUnicodeString &string, bool indent, bool newline ){
	const decString utf8TagName = tagName.ToUTF8();
	WriteDataTagString( utf8TagName.GetString(), string.ToUTF8(), indent, newline );
}

void decXmlWriter::WriteDataTagBool( const char *tagName, bool value,
bool indent, bool newline ){
	WriteOpeningTag( tagName, false, indent, false );
	WriteTextBool( value );
	WriteClosingTag( tagName, false, newline );
}

void decXmlWriter::WriteDataTagBool( const decUnicodeString &tagName, bool value,
bool indent, bool newline ){
	const decString utf8TagName( tagName.ToUTF8() );
	WriteDataTagBool( utf8TagName.GetString(), value, indent, newline );
}

void decXmlWriter::WriteDataTagInt( const char *tagName, int value,
bool indent, bool newline ){
	WriteOpeningTag( tagName, false, indent, false );
	WriteTextInt( value );
	WriteClosingTag( tagName, false, newline );
}

void decXmlWriter::WriteDataTagInt( const decUnicodeString &tagName, int value,
bool indent, bool newline ){
	const decString utf8TagName( tagName.ToUTF8() );
	WriteDataTagInt( utf8TagName.GetString(), value, indent, newline );
}

void decXmlWriter::WriteDataTagFloat( const char *tagName, float value,
bool indent, bool newline ){
	WriteOpeningTag( tagName, false, indent, false );
	WriteTextFloat( value );
	WriteClosingTag( tagName, false, newline );
}

void decXmlWriter::WriteDataTagFloat( const decUnicodeString &tagName,
float value, bool indent, bool newline ){
	const decString utf8TagName( tagName.ToUTF8() );
	WriteDataTagFloat( utf8TagName.GetString(), value, indent, newline );
}

void decXmlWriter::WriteDataTagDouble( const char *tagName, double value,
bool indent, bool newline ){
	WriteOpeningTag( tagName, false, indent, false );
	WriteTextDouble( value );
	WriteClosingTag( tagName, false, newline );
}

void decXmlWriter::WriteDataTagDouble( const decUnicodeString &tagName,
double value, bool indent, bool newline ){
	const decString utf8TagName( tagName.ToUTF8() );
	WriteDataTagDouble( utf8TagName.GetString(), value, indent, newline );
}



decString decXmlWriter::EscapeText( const char *text ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	
	const unsigned char *walker = ( const unsigned char * )text;
	decString escaped;
	
	while( *walker ){
		/*
		if( IS( *walker, 0x9 ) ||  IS( *walker, 0xA ) ||  IS( *walker, 0xD )
		||  ( *walker >= 0x20 && ! ( IN( *walker, 0x7f, 0x84 ) || IN( *walker, 0x86, 0x9f ) ) ) ){
		*/
		
		if( *walker == '<' || *walker == '&' || *walker == '>' ){
			escaped.AppendFormat( "&#%i;", *walker );
			
		}else{
			escaped.AppendCharacter( *walker );
		}
		
		walker++;
	}
	
	return escaped;
}
