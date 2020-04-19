/* 
 * Drag[en]gine Live Launcher
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

#include "dellGame.h"
#include "dellGameXML.h"
#include "fileformat/dellFileFormat.h"
#include "fileformat/dellFileFormatList.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class dellGameXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGameXML::dellGameXML( deLogger *logger, const char *loggerSource ) : dellBaseXML( logger, loggerSource ){
}

dellGameXML::~dellGameXML(){
}



// Management
///////////////

void dellGameXML::ReadFromFile( decBaseFileReader &reader, dellGame &game ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "degame" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadGame( *root, game );
}



// Private Functions
//////////////////////

void dellGameXML::pReadGame( const decXmlElementTag &root, dellGame &game ){
	dellFileFormatList &fileFormatList = game.GetFileFormatList();
	const int elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	int e;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "identifier" ) == 0 ){
				game.SetIdentifier( decUuid( pGetCDataString( *tag ), false ) );
				
			}else if( strcmp( tag->GetName(), "aliasIdentifier" ) == 0 ){
				game.SetAliasIdentifier( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "title" ) == 0 ){
				game.SetTitle( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "subTitle" ) == 0 ){
				game.SetTitle( game.GetTitle() + decUnicodeString::NewFromUTF8( " - " )
					+ decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "description" ) == 0 ){
				game.SetDescription( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "icon" ) == 0 ){
				const int size = pGetAttributeInt( *tag, "size" );
				if( size > game.GetIconSize() ){
					game.SetIcon( pGetCDataString( *tag ), size );
				}
				
			}else if( strcmp( tag->GetName(), "creator" ) == 0 ){
				game.SetCreator( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "homepage" ) == 0 ){
				game.SetHomepage( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "gameDirectory" ) == 0 ){
				game.SetGameDirectory( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "dataDirectory" ) == 0 ){
				game.SetDataDirectory( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "scriptDirectory" ) == 0 ){
				game.SetScriptDirectory( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "gameObject" ) == 0 ){
				game.SetGameObject( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "pathDefaultConfig" ) == 0 ){
				// deprecated
				
			}else if( strcmp( tag->GetName(), "pathConfig" ) == 0 ){
				game.SetPathConfig( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "pathCapture" ) == 0 ){
				game.SetPathCapture( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "scriptModule" ) == 0 ){
				game.SetScriptModule( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "windowSize" ) == 0 ){
				game.SetWindowSize( decPoint( pGetAttributeInt( *tag, "x" ),
					pGetAttributeInt( *tag, "y" ) ) );
				
			}else if( strcmp( tag->GetName(), "requireFormat" ) == 0 ){
				const decString typeName( pGetAttributeString( *tag, "type" ) );
				const deModuleSystem::eModuleTypes formatType = deModuleSystem::GetTypeFromString( typeName );
				if( formatType == deModuleSystem::emtUnknown ){
					pErrorUnknownValue( *tag, typeName );
					DETHROW( deeInvalidParam );
				}
				
				dellFileFormat *fileFormat = NULL;
				try{
					fileFormat = new dellFileFormat;
					fileFormat->SetType( formatType );
					fileFormat->SetPattern( pGetCDataString( *tag ) );
					fileFormatList.Add( fileFormat );
					fileFormat->FreeReference();
					
				}catch( const deException & ){
					if( fileFormat ){
						delete fileFormat;
					}
					throw;
				}
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
	
	if( ! game.GetIdentifier() ){
		pErrorMissingTag( root, "identifier" );
		DETHROW( deeInvalidParam );
	}
}
