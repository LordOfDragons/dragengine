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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decXmlDocument.h"
#include "decXmlContainer.h"
#include "decXmlElement.h"
#include "decXmlComment.h"
#include "decXmlPI.h"
#include "decXmlElementTag.h"
#include "decXmlCharacterData.h"
#include "decXmlEntityReference.h"
#include "decXmlCharReference.h"
#include "decXmlCDSect.h"
#include "decXmlAttValue.h"
#include "decXmlNamespace.h"
#include "decXmlParser.h"
#include "decXmlVisitor.h"
#include "../exceptions.h"
#include "../file/decBaseFileReader.h"
#include "../../logger/deLogger.h"
#include "../../dragengine_configuration.h"



// Class decXmlParser
///////////////////////

// Constructor, destructor
////////////////////////////

decXmlParser::decXmlParser( deLogger *logger ){
	if( ! logger ) DETHROW( deeInvalidParam );
	
	pLogger = logger;
	pLine = 1;
	pPos = 1;
	pToken = NULL;
	pTokenLen = 0;
	pTokenLine = 1;
	pTokenPos = 1;
	pCleanString = NULL;
	pCleanStringSize = 0;
	pFile = NULL;
	pCurChar = DEXP_EOF;
	pHasFatalError = false;
	
	try{
		pTokenSize = 256;
		pToken = new char[257];
		
	}catch( const deException & ){
		if( pToken ) delete [] pToken;
		throw;
	}
}

decXmlParser::~decXmlParser(){
	if( pCleanString ) delete [] pCleanString;
	if( pToken ) delete [] pToken;
}



// Management
///////////////

bool decXmlParser::ParseXml( decBaseFileReader *file, decXmlDocument *doc ){
	if( ! doc ) DETHROW( deeInvalidParam );
	PrepareParse( file );
	try{
		ParseDocument( doc );
	}catch( const deException & ){
		if( ! pHasFatalError ) throw;
	}
	return ! pHasFatalError;
}



// Error handling
///////////////////

void decXmlParser::UnexpectedEOF( int line, int pos ){
	pLogger->LogError( "XmlParser", "Unexpected end-of-file found." );
}

void decXmlParser::UnexpectedToken( int line, int pos, const char *token ){
	pLogger->LogErrorFormat( "XmlParser", "Unexpected token '%s' found at %i:%i.", token, line, pos );
}



// Parsing tokens
///////////////////

void decXmlParser::PrepareParse( decBaseFileReader *file ){
	if( ! file ) DETHROW( deeInvalidParam );
	pFile = file;
	pFilePos = file->GetPosition();
	pFileLen = file->GetLength();
	ClearToken();
	pLine = 1;
	pPos = 1;
	pTokenLine = 1;
	pTokenPos = 1;
	pCurChar = DEXP_EOF;
	pHasFatalError = false;
}

void decXmlParser::ParseDocument( decXmlDocument *doc ){
	// document ::= prolog element Misc*
	ParseProlog( doc );
	if( ! ParseElementTag( doc, doc->GetDocType() ) ) RaiseFatalError();
	ParseMisc( doc );
	if( GetTokenAt( 0 ) != DEXP_EOF ) RaiseFatalError();
}

void decXmlParser::ParseProlog( decXmlDocument *doc ){
	// prolog ::= XMLDecl? Misc* (doctypedecl Misc*)?
	ParseXMLDecl( doc );
	ParseMisc( doc );
	ParseDocTypeDecl( doc );
	ParseMisc( doc );
}

void decXmlParser::ParseXMLDecl( decXmlDocument *doc ){
	bool hasSpaces = false;
	// XMLDecl ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
	if( ! ParseToken( "<?xml" ) ) return;
	// VersionInfo ::= S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
	if( ParseSpaces() < 1 ) RaiseFatalError();
	if( ! ParseToken( "version" ) ) RaiseFatalError();
	ParseEquals();
	if( ! ( ParseToken( "'1.0'" ) || ParseToken( "\"1.0\"" ) ) ) RaiseFatalError();
	hasSpaces = ParseSpaces() > 0;
	// EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' | "'" EncName "'" )
	if( hasSpaces && ParseToken( "encoding" ) ){
		ParseEquals();
		ParseEncName( doc );
		hasSpaces = ParseSpaces() > 0;
	}
	// SDDecl ::= S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
	if( hasSpaces && ParseToken( "standalone" ) ){
		ParseEquals();
		if( ParseToken( "'yes'" ) || ParseToken( "\"yes\"" ) ){
			doc->SetStandalone( true );
		}else if( ParseToken( "'no'" ) || ParseToken( "\"no\"" ) ){
			doc->SetStandalone( false );
		}else{
			RaiseFatalError();
		}
		hasSpaces = ParseSpaces() > 0;
	}
	// S? '?>'
	if( ! ParseToken( "?>" ) ) RaiseFatalError();
}

void decXmlParser::ParseDocTypeDecl( decXmlDocument *doc ){
	// doctypedecl ::= '<!DOCTYPE' S Name (S ExternalID)? S? ('[' intSubset ']' S?)? '>'
	if( ! ParseToken( "<!DOCTYPE" ) ) return;
	if( ParseSpaces() < 1 ) RaiseFatalError();
	ParseName( 0, true );
	doc->SetDocType( pCleanString );
	if( ParseSpaces() > 0 ){
		// ExternalID ::= 'SYSTEM' S SystemLiteral | 'PUBLIC' S PubidLiteral S SystemLiteral
		if( ParseToken( "SYSTEM" ) ){
			if( ParseSpaces() == 0 ) RaiseFatalError();
			ParseSystemLiteral( doc );
		}else if( ParseToken( "PUBLIC" ) ){
			if( ParseSpaces() == 0 ) RaiseFatalError();
			ParsePublicLiteral( doc );
			if( ParseSpaces() == 0 ) RaiseFatalError();
			ParseSystemLiteral( doc );
		}else{
			RaiseFatalError();
		}
	}
	ParseSpaces();
	if( ParseToken( "[" ) ){
		// deprecated and not supported anymore
		RaiseFatalError();
	}
	// '>'
	if( ! ParseToken( ">" ) ) RaiseFatalError();
}

void decXmlParser::ParseSystemLiteral( decXmlDocument *doc ){
	int nextChar, count = 0;
	const char *delimiter;
	// SystemLiteral ::= ('"' [^"]* '"') | ("'" [^']* "'")
	if( ParseToken( "'" ) ){
		delimiter = "'";
	}else if( ParseToken( "\"" ) ){
		delimiter = "\"";
	}else{
		RaiseFatalError();
		return;
	}
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) RaiseFatalError();
		if( nextChar == *delimiter ) break;
		count++;
	}
	SetCleanString( count );
	doc->SetSystemLiteral( pCleanString );
	RemoveFromToken( count );
	if( ! ParseToken( delimiter ) ) RaiseFatalError();
}

void decXmlParser::ParsePublicLiteral( decXmlDocument *doc ){
	int nextChar, count = 0;
	const char *delimiter;
	bool restricted;
	// PubidLiteral ::= '"' PubidChar* '"' | "'" (PubidChar - "'")* "'"
	if( ParseToken( "'" ) ){
		delimiter = "'";
	}else if( ParseToken( "\"" ) ){
		delimiter = "\"";
	}else{
		RaiseFatalError();
		return;
	}
	restricted = delimiter[0] == '\'';
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) RaiseFatalError();
		if( ! IsPubidChar( nextChar, restricted ) ) break;
		count++;
	}
	SetCleanString( count );
	doc->SetPublicLiteral( pCleanString );
	RemoveFromToken( count );
	if( ! ParseToken( delimiter ) ) RaiseFatalError();
}

bool decXmlParser::ParseElementTag( decXmlContainer *container, const char *requiredName ){
	decXmlElementTag *tag = NULL;
	decXmlCharacterData *charData = NULL;
	int nextChar, count = 0;
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	
	// element ::= EmptyElemTag | STag content ETag
	// EmptyElemTag ::= '<' Name (S Attribute)* S? '/>'
	// STag ::= '<' Name (S Attribute)* S? '>'
	if( ! ParseToken( "<" ) ){
		return false;
	}
	ParseName( 0, true );
	if( requiredName && requiredName[ 0 ] && strcmp( requiredName, pCleanString ) != 0 ){
		RaiseFatalError();
	}
	tag = new decXmlElementTag( pCleanString );
	tag->SetLineNumber( lineNumber );
	tag->SetPositionNumber( posNumber );
	
	try{
		while( true ){
			ParseSpaces();
			
			if( ParseToken( ">" ) ){
				while( true ){
					// content ::= CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
					// CharData ::= [^<&]* - ([^<&]* ']]>' [^<&]*)
					lineNumber = pTokenLine;
					posNumber = pTokenPos;
					count = 0;
					while( true ){
						nextChar = GetTokenAt( count );
						if( nextChar == DEXP_EOF ) RaiseFatalError();
						if( nextChar == '<' || nextChar == '&' ){
							break;
						}
						count++;
					}
					if( count > 0 ){
						SetCleanString( count );
						pAddCharacterData( tag, pCleanString, lineNumber, posNumber );
						//charData = new decXmlCharacterData( pCleanString );
						//if( ! charData ) DETHROW( deeOutOfMemory );
						//charData->SetLineNumber( lineNumber );
						//charData->SetPositionNumber( posNumber );
						//tag->AddElement( charData );
						//charData = NULL;
						RemoveFromToken( count );
					}
					
					// (element | Reference | CDSect | PI | Comment)
					if( ParseToken( "</" ) ){
						break;
					}
					if( ! ( ParseComment( tag ) || ParsePI( tag ) || ParseReference( tag )
					|| ParseCDSect( tag ) || ParseElementTag( tag, NULL ) ) ){
						RaiseFatalError();
					}
				}
				
				// ETag ::= '</' Name S? '>'
				ParseName( 0, true );
				if( strcmp( pCleanString, tag->GetName() ) != 0 ){
					RaiseFatalError();
				}
				ParseSpaces();
				if( ! ParseToken( ">" ) ){
					RaiseFatalError();
				}
				break;
				
			}else if( ParseToken( "/>" ) ){
				break;
				
			}else{
				ParseAttribute( tag );
			}
		}
		
		container->AddElement( tag );
		tag->FreeReference();
		
	}catch( const deException & ){
		if( charData ){
			charData->FreeReference();
		}
		tag->FreeReference();
		throw;
	}
	return true;
}

bool decXmlParser::ParseReference( decXmlContainer *container ){
	decXmlEntityReference *entRef = NULL;
	decXmlCharReference *charRef = NULL;
	int nextChar, count = 0;
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	int character = 0;
	int tchar;
	
	// Reference ::= EntityRef | CharRef
	// EntityRef ::= '&' Name ';'
	// CharRef ::= '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
	try{
		nextChar = GetTokenAt( 0 );
		if( nextChar == '&' ){
			nextChar = GetTokenAt( 1 );
			if( nextChar == '#' ){
				nextChar = GetTokenAt( 2 );
				if( nextChar == 'x' ){
					character = 0;
					count = 3;
					tchar = GetTokenAt( count++ );
					if( ! IsHex( tchar ) ) RaiseFatalError();
					
					while( true ){
						if( tchar >= 'A' && tchar <= 'F' ){
							character = ( character << 4 ) + 10 + ( tchar - 'A' );
							
						}else if( tchar >= 'a' && tchar <= 'f' ){
							character = ( character << 4 ) + 10 + ( tchar - 'a' );
							
						}else{
							character = ( character << 4 ) + ( tchar - '0' );
						}
						
						tchar = GetTokenAt( count );
						if( ! IsHex( tchar ) ) break;
						count++;
					}
					if( GetTokenAt( count ) != ';' ) RaiseFatalError();
					SetCleanString( count );
					if( character ){
						pAddCharacterData( container, ( char )character, lineNumber, posNumber );
					}else{
//						charRef = new decXmlCharReference( pCleanString + 3, decXmlCharReference::erHexadecimal );
//						if( ! charRef ) DETHROW( deeOutOfMemory );
					}
				}else{
					character = 0;
					count = 2;
					tchar = GetTokenAt( count++ );
					if( ! IsLatinDigit( tchar ) ) RaiseFatalError();
					
					while( true ){
						character = character * 10 + ( tchar - '0' );
						
						tchar = GetTokenAt( count );
						if( ! IsLatinDigit( tchar ) ) break;
						count++;
					}
					if( GetTokenAt( count ) != ';' ) RaiseFatalError();
					SetCleanString( count );
					if( character ){
						pAddCharacterData( container, ( char )character, lineNumber, posNumber );
					}else{
//						charRef = new decXmlCharReference( pCleanString + 2, decXmlCharReference::erDecimal );
//						if( ! charRef ) DETHROW( deeOutOfMemory );
					}
				}
				if( charRef ){
					charRef->SetLineNumber( lineNumber );
					charRef->SetPositionNumber( posNumber );
					container->AddElement( charRef );
					charRef->FreeReference();
					charRef = NULL;
				}
				
			}else{
				count = ParseName( count + 1, false );
				if( GetTokenAt( count ) != ';' ) RaiseFatalError();
				SetCleanString( count );
				
				const char * const name = pCleanString + 1;
				if( strcmp( name, "lt" ) == 0 ){
					pAddCharacterData( container, '<', lineNumber, posNumber );
					
				} else if( strcmp( name, "gt" ) == 0 ){
					pAddCharacterData( container, '>', lineNumber, posNumber );
					
				} else if( strcmp( name, "amp" ) == 0 ){
					pAddCharacterData( container, '&', lineNumber, posNumber );
					
				} else if( strcmp( name, "quot" ) == 0 ){
					pAddCharacterData( container, '"', lineNumber, posNumber );
					
				} else if( strcmp( name, "apos" ) == 0 ){
					pAddCharacterData( container, '\'', lineNumber, posNumber );
					
				} else {
					entRef = new decXmlEntityReference( name );
					entRef->SetLineNumber( lineNumber );
					entRef->SetPositionNumber( posNumber );
					container->AddElement( entRef );
					entRef->FreeReference();
					entRef = nullptr;
				}
			}
			
			RemoveFromToken( count + 1 );
		}else{
			return false;
		}
	}catch( const deException & ){
		if( charRef ){
			charRef->FreeReference();
		}
		if( entRef ){
			entRef->FreeReference();
		}
		throw;
	}
	return true;
}

bool decXmlParser::ParseCDSect( decXmlContainer *container ){
	decXmlCDSect *cdsect = NULL;
	int nextChar, count = 0;
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	// CDSect ::= CDStart CData CDEnd
	// CDStart ::= '<![CDATA['
	// CData ::= (Char* - (Char* ']]>' Char*))
	// CDEnd ::= ']]>'
	if( ! ParseToken( "<![CDATA[" ) ) return false;
	try{
		while( true ){
			nextChar = GetTokenAt( count );
			if( nextChar == DEXP_EOF ) RaiseFatalError();
			if( TestToken( count, "]]>" ) ) break;
			count++;
		}
		SetCleanString( count );
		cdsect = new decXmlCDSect( pCleanString );
		if( ! cdsect ) DETHROW( deeOutOfMemory );
		cdsect->SetLineNumber( lineNumber );
		cdsect->SetPositionNumber( posNumber );
		RemoveFromToken( count );
		container->AddElement( cdsect );
		cdsect->FreeReference();
		cdsect = NULL;
		if( ! ParseToken( "]]>" ) ) RaiseFatalError();
	}catch( const deException & ){
		if( cdsect ){
			cdsect->FreeReference();
		}
		throw;
	}
	return true;
}

void decXmlParser::ParseAttribute( decXmlContainer *container ){
	decXmlAttValue *attValue = NULL;
	decXmlNamespace *ns = NULL;
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	// Attribute ::= Name Eq AttValue
	// (ADDITION): if Name begins with 'xmlns:' make a namespace out of it
	ParseName( 0, true );
	attValue = new decXmlAttValue( pCleanString );
	if( ! attValue ) DETHROW( deeOutOfMemory );
	attValue->SetLineNumber( lineNumber );
	attValue->SetPositionNumber( posNumber );
	try{
		ParseEquals();
		ParseAttValue( attValue );
		// check if this is a namespace
		if( strncmp( attValue->GetName(), "xmlns:", 6 ) == 0 ){
			ns = new decXmlNamespace( attValue->GetName() + 6, attValue->GetValue() );
			if( ! ns ) DETHROW( deeOutOfMemory );
			ns->SetLineNumber( lineNumber );
			ns->SetPositionNumber( posNumber );
			attValue->FreeReference();
			attValue = NULL;
			container->AddElement( ns );
			ns->FreeReference();
		}else{
			container->AddElement( attValue );
			attValue->FreeReference();
		}
	}catch( const deException & ){
		if( ns ){
			ns->FreeReference();
		}
		if( attValue ){
			attValue->FreeReference();
		}
		throw;
	}
}

void decXmlParser::ParseAttValue( decXmlAttValue *value ){
	int delimiter, nextChar, count = 0;
	int character = 0;
	int safeguard;
	
	// AttValue ::= '"' ([^<&"] | Reference)* '"' | "'" ([^<&'] | Reference)* "'"
	// Reference ::= EntityRef | CharRef
	// EntityRef ::= '&' Name ';'
	// CharRef ::= '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
	if( ParseToken( "\"" ) ){
		delimiter = '"';
	}else if( ParseToken( "'" ) ){
		delimiter = '\'';
	}else{
		RaiseFatalError();
		return;
	}
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) RaiseFatalError();
		if( nextChar == '<' ) RaiseFatalError();
		if( nextChar == delimiter ) break;
		if( nextChar == '&' ){
			nextChar = GetTokenAt( count + 1 );
			if( nextChar == '#' ){
				safeguard = count;
				character = 0;
				nextChar = GetTokenAt( count + 2 );
				if( nextChar == 'x' ){
					count += 3;
					nextChar = GetTokenAt( count++ );
					if( ! IsHex( nextChar ) ) RaiseFatalError();
					while( true ){
						if( nextChar >= 'A' && nextChar <= 'F' ){
							character = ( character << 4 ) + 10 + ( nextChar - 'A' );
							
						}else if( nextChar >= 'a' && nextChar <= 'f' ){
							character = ( character << 4 ) + 10 + ( nextChar - 'a' );
							
						}else{
							character = ( character << 4 ) + ( nextChar - '0' );
						}
						
						nextChar = GetTokenAt( count );
						if( ! IsHex( nextChar ) ) break;
						count++;
					}
				}else{
					count += 2;
					nextChar = GetTokenAt( count++ );
					if( ! IsLatinDigit( nextChar ) ) RaiseFatalError();
					while( true ){
						character = character * 10 + ( nextChar - '0' );
						
						nextChar = GetTokenAt( count );
						if( ! IsLatinDigit( nextChar ) ) break;
						count++;
					}
				}
				if( GetTokenAt( count ) != ';' ) RaiseFatalError();
				pTokenLen = safeguard;
				if( character ){
					AddCharToToken( character );
				}
				count = pTokenLen;
			}else{
				safeguard = count;
				count = ParseName( count + 1, false );
				if( GetTokenAt( count ) != ';' ) RaiseFatalError();
				
				const char * const name = pToken + safeguard + 1;
				if( strncmp( name, "lt", 2 ) == 0 ){
					pToken[ safeguard ] = '<';
					count = pTokenLen = safeguard + 1;
					
				} else if( strncmp( name, "gt", 2 ) == 0 ){
					pToken[ safeguard ] = '>';
					count = pTokenLen = safeguard + 1;
					
				} else if( strncmp( name, "amp", 3 ) == 0 ){
					pToken[ safeguard ] = '&';
					count = pTokenLen = safeguard + 1;
					
				} else if( strncmp( name, "quot", 4 ) == 0 ){
					pToken[ safeguard ] = '"';
					count = pTokenLen = safeguard + 1;
					
				} else if( strncmp( name, "apos", 4 ) == 0 ){
					pToken[ safeguard ] = '\'';
					count = pTokenLen = safeguard + 1;
				}
			}
		}
		count++;
	}
	SetCleanString( count );
	value->SetValue( pCleanString );
	RemoveFromToken( count + 1 );
}

bool decXmlParser::ParseToken( const char *expected ){
	int i, nextChar, expLen = ( int )strlen( expected );
	for( i=0; i<expLen; i++ ){
		nextChar = GetTokenAt( i );
		if( nextChar == DEXP_EOF || nextChar != expected[ i ] ) return false;
	}
	RemoveFromToken( expLen );
	return true;
}

int decXmlParser::ParseSpaces(){
	int nextChar, count = 0;
	// S ::= (#x20 | #x9 | #xD | #xA)+
	while( true ){
		nextChar = GetTokenAt( 0 );
		if( nextChar == DEXP_EOF || ! IsSpace( nextChar ) ) break;
		RemoveFromToken( 1 );
		count++;
	}
	return count;
}

void decXmlParser::ParseEquals(){
	// Eq ::= S? '=' S?
	if( ParseSpaces() > 1 ) RaiseFatalError();
	if( ! ParseToken( "=" ) ) RaiseFatalError();
	if( ParseSpaces() > 1 ) RaiseFatalError();
}

void decXmlParser::ParseEncName( decXmlDocument *doc ){
	int nextChar, count = 0;
	const char *delimiter;
	// EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')*
	if( ParseToken( "'" ) ){
		delimiter = "'";
	}else if( ParseToken( "\"" ) ){
		delimiter = "\"";
	}else{
		RaiseFatalError();
		return;
	}
	nextChar = GetTokenAt( 0 );
	if( nextChar == DEXP_EOF || ! IsLatinLetter( nextChar ) ) RaiseFatalError();
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) RaiseFatalError();
		if( ! ( IsLatinLetter( nextChar ) || IsLatinDigit( nextChar )
		|| nextChar == '-' || nextChar == '.' || nextChar == '_' ) ){
			break;
		}
		count++;
	}
	SetCleanString( count );
	doc->SetEncoding( pCleanString );
	RemoveFromToken( count );
	if( ! ParseToken( delimiter ) ) RaiseFatalError();
}

void decXmlParser::ParseMisc( decXmlContainer *container ){
	// Misc ::= Comment | PI | S
	while( ParseSpaces() || ParseComment( container ) || ParsePI( container ) );
}

bool decXmlParser::ParseComment( decXmlContainer *container ){
	// Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	int nextChar, count = 0;
	decXmlComment *comment = NULL;
	if( ! ParseToken( "<!--" ) ) return false;
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) RaiseFatalError();
		if( nextChar == '-' ){
			nextChar = GetTokenAt( count + 1 );
			if( nextChar == DEXP_EOF ) RaiseFatalError();
			if( nextChar == '-' ){
				nextChar = GetTokenAt( count + 2 );
				if( nextChar != '>' ) RaiseFatalError();
				break;
			}
// 			if( ! IsChar( nextChar ) ) RaiseFatalError();
			count++;
		}else{
// 			if( ! IsChar( nextChar ) ) RaiseFatalError();
		}
		count++;
	}
	// add comment
	SetCleanString( count );
	comment = new decXmlComment( pCleanString );
	if( ! comment ) DETHROW( deeOutOfMemory );
	comment->SetLineNumber( lineNumber );
	comment->SetPositionNumber( posNumber );
	try{
		container->AddElement( comment );
		comment->FreeReference();
	}catch( const deException & ){
		comment->FreeReference();
		throw;
	}
	RemoveFromToken( count + 3 );
	return true;
}

bool decXmlParser::ParsePI( decXmlContainer *container ){
	// PI ::= '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'
	// PITarget ::= Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
	int lineNumber = pTokenLine;
	int posNumber = pTokenPos;
	int nextChar, count = 0;
	decXmlPI *pi = NULL;
	if( ! ParseToken( "<?" ) ) return false;
	ParseName( 0, true );
	if( strlen( pCleanString ) >= 3 ){
		if( pCleanString[ 0 ] == 'X' || pCleanString[ 0 ] == 'x'
		|| pCleanString[ 1 ] == 'M' || pCleanString[ 1 ] == 'm'
		|| pCleanString[ 2 ] == 'L' || pCleanString[ 2 ] == 'l' ){
			RaiseFatalError();
		}
	}
	pi = new decXmlPI( pCleanString );
	if( ! pi ) DETHROW( deeOutOfMemory );
	pi->SetLineNumber( lineNumber );
	pi->SetPositionNumber( posNumber );
	try{
		if( ParseSpaces() > 0 ){
			while( true ){
				nextChar = GetTokenAt( count );
				if( nextChar == DEXP_EOF ) RaiseFatalError();
				if( nextChar == '?' ){
					nextChar = GetTokenAt( count + 1 );
					if( nextChar == DEXP_EOF ) RaiseFatalError();
					if( nextChar == '>' ) break;
// 					if( ! IsChar( nextChar ) ) RaiseFatalError();
					count++;
				}else{
// 					if( ! IsChar( nextChar ) ) RaiseFatalError();
				}
				count++;
			}
			SetCleanString( count );
			pi->SetCommand( pCleanString );
			RemoveFromToken( count + 2 );
		}
		container->AddElement( pi );
		pi->FreeReference();
	}catch( const deException & ){
		pi->FreeReference();
		throw;
	}
	return true;
}

bool decXmlParser::TestToken( int offset, const char *expected ){
	int i, nextChar, expLen = ( int )strlen( expected );
	for( i=0; i<expLen; i++ ){
		nextChar = GetTokenAt( offset + i );
		if( nextChar == DEXP_EOF || nextChar != expected[ i ] ) return false;
	}
	return true;
}

int decXmlParser::ParseName( int offset, bool autoRemove ){
	if( offset < 0 ) DETHROW( deeInvalidParam );
	int nextChar, count = offset;
	// Name ::= (Letter | '_' | ':') (NameChar)*
	// NameChar ::= Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
	nextChar = GetTokenAt( count );
	if( nextChar == DEXP_EOF ) RaiseFatalError();
	if( ! IsLetter( nextChar ) && nextChar != '_' && nextChar != ':' ) RaiseFatalError();
	while( true ){
		nextChar = GetTokenAt( count );
		if( nextChar == DEXP_EOF ) break;
		if( ! IsLetter( nextChar ) && ! IsDigit( nextChar ) && nextChar != '.'
		&& nextChar != '-' && nextChar != '_' && nextChar != ':'
		&& ! IsCombiningChar( nextChar ) && ! IsExtender( nextChar ) ){
			break;
		}
		count++;
	}
	if( autoRemove ){
		SetCleanString( count );
		RemoveFromToken( count );
	}
	return count;
}



// Testing chars
//////////////////

bool decXmlParser::IsLatinLetter( int aChar ){
	// LatinLetter ::= [A-Za-z]
	return ( aChar >= 'A' && aChar <= 'Z' ) || ( aChar >= 'a' && aChar <= 'z' );
}

bool decXmlParser::IsLatinDigit( int aChar ){
	// Digit ::= [0-9]
	return aChar >= '0' && aChar <= '9';
}

bool decXmlParser::IsHex( int aChar ){
	// Hex ::= [0-9a-fA-F]
	return ( aChar >= '0' && aChar <= '9' ) || ( aChar >= 'a' && aChar <= 'f' )
		|| ( aChar >= 'A' && aChar <= 'F' );
}

bool decXmlParser::IsLetter( int aChar ){
	// Letter ::= BaseChar | Ideographic
	return IsBaseChar( aChar ) || IsIdeographic( aChar );
}

bool decXmlParser::IsDigit( int aChar ){
	// Digit ::= [#x0030-#x0039] | [#x0660-#x0669] | [#x06F0-#x06F9] | [#x0966-#x096F]
	// | [#x09E6-#x09EF] | [#x0A66-#x0A6F] | [#x0AE6-#x0AEF] | [#x0B66-#x0B6F] | [#x0BE7-#x0BEF]
	// | [#x0C66-#x0C6F] | [#x0CE6-#x0CEF] | [#x0D66-#x0D6F] | [#x0E50-#x0E59] | [#x0ED0-#x0ED9]
	// | [#x0F20-#x0F29]
	return aChar >= 0x30 && aChar <= 0x39;
}

bool decXmlParser::IsBaseChar( int aChar ){
	// BaseChar ::= [#x0041-#x005A] | [#x0061-#x007A] | [#x00C0-#x00D6] | [#x00D8-#x00F6]
	// | [#x00F8-#x00FF] | [#x0100-#x0131] | [#x0134-#x013E] | [#x0141-#x0148] | [#x014A-#x017E]
	// | [#x0180-#x01C3] | [#x01CD-#x01F0] | [#x01F4-#x01F5] | [#x01FA-#x0217] | [#x0250-#x02A8]
	// | [#x02BB-#x02C1] | #x0386 | [#x0388-#x038A] | #x038C | [#x038E-#x03A1] | [#x03A3-#x03CE]
	// | [#x03D0-#x03D6] | #x03DA | #x03DC | #x03DE | #x03E0 | [#x03E2-#x03F3] | [#x0401-#x040C]
	// | [#x040E-#x044F] | [#x0451-#x045C] | [#x045E-#x0481] | [#x0490-#x04C4] | [#x04C7-#x04C8]
	// | [#x04CB-#x04CC] | [#x04D0-#x04EB] | [#x04EE-#x04F5] | [#x04F8-#x04F9] | [#x0531-#x0556]
	// | #x0559 | [#x0561-#x0586] | [#x05D0-#x05EA] | [#x05F0-#x05F2] | [#x0621-#x063A]
	// | [#x0641-#x064A] | [#x0671-#x06B7] | [#x06BA-#x06BE] | [#x06C0-#x06CE] | [#x06D0-#x06D3]
	// | #x06D5 | [#x06E5-#x06E6] | [#x0905-#x0939] | #x093D | [#x0958-#x0961] | [#x0985-#x098C]
	// | [#x098F-#x0990] | [#x0993-#x09A8] | [#x09AA-#x09B0] | #x09B2 | [#x09B6-#x09B9]
	// | [#x09DC-#x09DD] | [#x09DF-#x09E1] | [#x09F0-#x09F1] | [#x0A05-#x0A0A] | [#x0A0F-#x0A10]
	// | [#x0A13-#x0A28] | [#x0A2A-#x0A30] | [#x0A32-#x0A33] | [#x0A35-#x0A36] | [#x0A38-#x0A39]
	// | [#x0A59-#x0A5C] | #x0A5E | [#x0A72-#x0A74] | [#x0A85-#x0A8B] | #x0A8D | [#x0A8F-#x0A91]
	// | [#x0A93-#x0AA8] | [#x0AAA-#x0AB0] | [#x0AB2-#x0AB3] | [#x0AB5-#x0AB9] | #x0ABD | #x0AE0
	// | [#x0B05-#x0B0C] | [#x0B0F-#x0B10] | [#x0B13-#x0B28] | [#x0B2A-#x0B30] | [#x0B32-#x0B33]
	// | [#x0B36-#x0B39] | #x0B3D | [#x0B5C-#x0B5D] | [#x0B5F-#x0B61] | [#x0B85-#x0B8A]
	// | [#x0B8E-#x0B90] | [#x0B92-#x0B95] | [#x0B99-#x0B9A] | #x0B9C | [#x0B9E-#x0B9F]
	// | [#x0BA3-#x0BA4] | [#x0BA8-#x0BAA] | [#x0BAE-#x0BB5] | [#x0BB7-#x0BB9] | [#x0C05-#x0C0C]
	// | [#x0C0E-#x0C10] | [#x0C12-#x0C28] | [#x0C2A-#x0C33] | [#x0C35-#x0C39] | [#x0C60-#x0C61]
	// | [#x0C85-#x0C8C] | [#x0C8E-#x0C90] | [#x0C92-#x0CA8] | [#x0CAA-#x0CB3] | [#x0CB5-#x0CB9]
	// | #x0CDE | [#x0CE0-#x0CE1] | [#x0D05-#x0D0C] | [#x0D0E-#x0D10] | [#x0D12-#x0D28]
	// | [#x0D2A-#x0D39] | [#x0D60-#x0D61] | [#x0E01-#x0E2E] | #x0E30 | [#x0E32-#x0E33]
	// | [#x0E40-#x0E45] | [#x0E81-#x0E82] | #x0E84 | [#x0E87-#x0E88] | #x0E8A | #x0E8D
	// | [#x0E94-#x0E97] | [#x0E99-#x0E9F] | [#x0EA1-#x0EA3] | #x0EA5 | #x0EA7 | [#x0EAA-#x0EAB]
	// | [#x0EAD-#x0EAE] | #x0EB0 | [#x0EB2-#x0EB3] | #x0EBD | [#x0EC0-#x0EC4] | [#x0F40-#x0F47]
	// | [#x0F49-#x0F69] | [#x10A0-#x10C5] | [#x10D0-#x10F6] | #x1100 | [#x1102-#x1103]
	// | [#x1105-#x1107] | #x1109 | [#x110B-#x110C] | [#x110E-#x1112] | #x113C | #x113E | #x1140
	// | #x114C | #x114E | #x1150 | [#x1154-#x1155] | #x1159 | [#x115F-#x1161] | #x1163 | #x1165
	// | #x1167 | #x1169 | [#x116D-#x116E] | [#x1172-#x1173] | #x1175 | #x119E | #x11A8 | #x11AB
	// | [#x11AE-#x11AF] | [#x11B7-#x11B8] | #x11BA | [#x11BC-#x11C2] | #x11EB | #x11F0 | #x11F9
	// | [#x1E00-#x1E9B] | [#x1EA0-#x1EF9] | [#x1F00-#x1F15] | [#x1F18-#x1F1D] | [#x1F20-#x1F45]
	// | [#x1F48-#x1F4D] | [#x1F50-#x1F57] | #x1F59 | #x1F5B | #x1F5D | [#x1F5F-#x1F7D]
	// | [#x1F80-#x1FB4] | [#x1FB6-#x1FBC] | #x1FBE | [#x1FC2-#x1FC4] | [#x1FC6-#x1FCC]
	// | [#x1FD0-#x1FD3] | [#x1FD6-#x1FDB] | [#x1FE0-#x1FEC] | [#x1FF2-#x1FF4] | [#x1FF6-#x1FFC]
	// | #x2126 | [#x212A-#x212B] | #x212E | [#x2180-#x2182] | [#x3041-#x3094] | [#x30A1-#x30FA]
	// | [#x3105-#x312C] | [#xAC00-#xD7A3] 
	return ( aChar >= 0x41 && aChar <= 0x5A ) || ( aChar >= 0x61 && aChar <= 0x7A )
		|| ( aChar >= 0xC0 && aChar <= 0xD6 ) || ( aChar >= 0xD8 && aChar <= 0xF6 )
		|| ( aChar >= 0xF8 && aChar <= 0xFF );
}

bool decXmlParser::IsCombiningChar( int aChar ){
	// CombiningChar ::= [#x0300-#x0345] | [#x0360-#x0361] | [#x0483-#x0486] | [#x0591-#x05A1]
	// | [#x05A3-#x05B9] | [#x05BB-#x05BD] | #x05BF | [#x05C1-#x05C2] | #x05C4 | [#x064B-#x0652]
	// | #x0670 | [#x06D6-#x06DC] | [#x06DD-#x06DF] | [#x06E0-#x06E4] | [#x06E7-#x06E8]
	// | [#x06EA-#x06ED] | [#x0901-#x0903] | #x093C | [#x093E-#x094C] | #x094D | [#x0951-#x0954]
	// | [#x0962-#x0963] | [#x0981-#x0983] | #x09BC | #x09BE | #x09BF | [#x09C0-#x09C4]
	// | [#x09C7-#x09C8] | [#x09CB-#x09CD] | #x09D7 | [#x09E2-#x09E3] | #x0A02 | #x0A3C | #x0A3E
	// | #x0A3F | [#x0A40-#x0A42] | [#x0A47-#x0A48] | [#x0A4B-#x0A4D] | [#x0A70-#x0A71]
	// | [#x0A81-#x0A83] | #x0ABC | [#x0ABE-#x0AC5] | [#x0AC7-#x0AC9] | [#x0ACB-#x0ACD]
	// | [#x0B01-#x0B03] | #x0B3C | [#x0B3E-#x0B43] | [#x0B47-#x0B48] | [#x0B4B-#x0B4D]
	// | [#x0B56-#x0B57] | [#x0B82-#x0B83] | [#x0BBE-#x0BC2] | [#x0BC6-#x0BC8] | [#x0BCA-#x0BCD]
	// | #x0BD7 | [#x0C01-#x0C03] | [#x0C3E-#x0C44] | [#x0C46-#x0C48] | [#x0C4A-#x0C4D]
	// | [#x0C55-#x0C56] | [#x0C82-#x0C83] | [#x0CBE-#x0CC4] | [#x0CC6-#x0CC8] | [#x0CCA-#x0CCD]
	// | [#x0CD5-#x0CD6] | [#x0D02-#x0D03] | [#x0D3E-#x0D43] | [#x0D46-#x0D48] | [#x0D4A-#x0D4D]
	// | #x0D57 | #x0E31 | [#x0E34-#x0E3A] | [#x0E47-#x0E4E] | #x0EB1 | [#x0EB4-#x0EB9]
	// | [#x0EBB-#x0EBC] | [#x0EC8-#x0ECD] | [#x0F18-#x0F19] | #x0F35 | #x0F37 | #x0F39 | #x0F3E
	// | #x0F3F | [#x0F71-#x0F84] | [#x0F86-#x0F8B] | [#x0F90-#x0F95] | #x0F97 | [#x0F99-#x0FAD]
	// | [#x0FB1-#x0FB7] | #x0FB9 | [#x20D0-#x20DC] | #x20E1 | [#x302A-#x302F] | #x3099 | #x309A 
	return false;
}

bool decXmlParser::IsIdeographic( int aChar ){
	// Ideographic ::= [#x4E00-#x9FA5] | #x3007 | [#x3021-#x3029]
	return false;
}

bool decXmlParser::IsExtender( int aChar ){
	// Extender ::= #x00B7 | #x02D0 | #x02D1 | #x0387 | #x0640 | #x0E46 | #x0EC6 | #x3005
	// | [#x3031-#x3035] | [#x309D-#x309E] | [#x30FC-#x30FE]
	return aChar == 0xB7;
}

bool decXmlParser::IsChar( int aChar ){
	// Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF]
	return aChar == 0x9 || aChar == 0xA || aChar == 0xD
		|| ( aChar >= 0x20 && aChar <= 0xFF );
}

bool decXmlParser::IsPubidChar( int aChar, bool restricted ){
	// PubidChar ::= #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
	// ( if restricted ' is not allowed )
	if( restricted && aChar == '\'' ) return false;
	return aChar == 0x20 || aChar == 0xD || aChar == 0xA || IsLatinLetter( aChar )
		|| IsLatinDigit( aChar ) || strchr( "-'()+,./:=?;!*#@$_%", aChar );
}

bool decXmlParser::IsSpace( int aChar ){
	return aChar == 0x20 || aChar == 0x9 || aChar == 0xD || aChar == 0xA;
}



// Reading tokens
///////////////////

int decXmlParser::GetTokenAt( int index ){
	while( pTokenLen <= index ){
		pGetNextCharAndAdd();
		if( pCurChar == DEXP_EOF ) return DEXP_EOF;
	}
	return pToken[ index ];
}

void decXmlParser::ClearToken(){
	int i;
	for( i=0; i<pTokenLen; i++ ){
		if( pToken[ i ] == '\n' ){
			pTokenLine++;
			pTokenPos = 0;
		}
		pTokenPos++;
	}
	pToken[ 0 ] = '\0';
	pTokenLen = 0;
}

void decXmlParser::AddCharToToken( int aChar ){
	if( pTokenLen == pTokenSize ) pGrowToken();
	pToken[ pTokenLen ] = (char)aChar;
	pToken[ pTokenLen + 1 ] = '\0';
	pTokenLen++;
}

void decXmlParser::RemoveFromToken( int length ){
	if( length == 0 ) return;
	if( length > pTokenLen ) DETHROW( deeInvalidParam );
	int i, copyLen = pTokenLen - length;
	for( i=0; i<length; i++ ){
		if( pToken[ i ] == '\n' ){
			pTokenLine++;
			pTokenPos = 0;
		}
		pTokenPos++;
	}
	for( i=0; i<copyLen; i++ ){
		pToken[ i ] = pToken[ i + length ];
	}
	pToken[ copyLen ] = '\0';
	pTokenLen = copyLen;
}

bool decXmlParser::IsEOF(){
	return pFilePos >= pFileLen;
}

void decXmlParser::RaiseFatalError(){
	if( pTokenLen ){
		UnexpectedToken( pTokenLine, pTokenPos, pToken );
	}else{
		UnexpectedEOF( pTokenLine, pTokenPos );
	}
	pHasFatalError = true;
	DETHROW( deeInvalidAction );
}

void decXmlParser::SetCleanString( int length ){
	if( length > pTokenLen ) DETHROW( deeInvalidParam );
	if( length > pCleanStringSize ){
		char *newStr = new char[ length + 1 ];
		if( ! newStr ) DETHROW( deeOutOfMemory );
		if( pCleanString ) delete [] pCleanString;
		pCleanString = newStr;
		pCleanStringSize = length;
	}
	#ifdef OS_W32_VS
		strncpy_s( pCleanString, length + 1, pToken, length );
	#else
		strncpy( pCleanString, pToken, length );
	#endif
	pCleanString[ length ] = '\0';
}



// Private Functions
//////////////////////

void decXmlParser::pGetNextChar(){
	if( IsEOF() ){
		pCurChar = DEXP_EOF;
	}else{
		if( pCurChar == '\n' ){
			pLine++;
			pPos = 0;
		}
		pCurChar = pFile->ReadChar();
		pFilePos++;
		pPos++;
	}
}

void decXmlParser::pGetNextCharAndAdd(){
	if( IsEOF() ){
		pCurChar = DEXP_EOF;
	}else{
		if( pCurChar == '\n' ){
			pLine++;
			pPos = 0;
		}
		pCurChar = pFile->ReadChar();
		AddCharToToken( pCurChar );
		pFilePos++;
		pPos++;
	}
}

void decXmlParser::pGrowToken(){
	int newSize = pTokenSize * 3 / 2 + 1;
	char *newToken = new char[ newSize + 1 ];
	if( ! newToken ) DETHROW( deeOutOfMemory );
	if( pToken ){
		if( pTokenLen > 0 ){
			#ifdef OS_W32_VS
				strncpy_s( newToken, pTokenLen + 1, pToken, pTokenLen );
			#else
				strncpy( newToken, pToken, pTokenLen );
			#endif
		}
		newToken[ pTokenLen ] = '\0';
		delete [] pToken;
	}
	pToken = newToken;
	pTokenSize = newSize;
}

void decXmlParser::pAddCharacterData( decXmlContainer *container, const char *text, int line, int pos ){
	int count = container->GetElementCount();
	decXmlCharacterData *cdata = NULL;
	decXmlElement *element = NULL;
	
	if( count > 0 ){
		element = container->GetElementAt( count - 1 );
		if( element->CanCastToCharacterData() ){
			element->CastToCharacterData()->AppendData( text );
			return;
		}
	}
	
	cdata = new decXmlCharacterData( text );
	if( ! cdata ) DETHROW( deeOutOfMemory );
	cdata->SetLineNumber( line );
	cdata->SetPositionNumber( pos );
	container->AddElement( cdata );
	cdata->FreeReference();
}

void decXmlParser::pAddCharacterData( decXmlContainer *container, char character, int line, int pos ){
	int count = container->GetElementCount();
	decXmlCharacterData *cdata = NULL;
	decXmlElement *element = NULL;
	
	if( count > 0 ){
		element = container->GetElementAt( count - 1 );
		if( element->CanCastToCharacterData() ){
			element->CastToCharacterData()->AppendCharacter( character );
			return;
		}
	}
	
	char buffer[ 2 ] = { character, '\0' };
	cdata = new decXmlCharacterData( ( const char * )&buffer );
	if( ! cdata ) DETHROW( deeOutOfMemory );
	cdata->SetLineNumber( line );
	cdata->SetPositionNumber( pos );
	container->AddElement( cdata );
	cdata->FreeReference();
}
