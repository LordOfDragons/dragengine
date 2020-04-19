/* 
 * Drag[en]gine Android Launcher
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

#include "dealGame.h"
#include "dealGameXML.h"
#include "fileformat/dealFileFormat.h"
#include "fileformat/dealFileFormatList.h"
#include "../engine/modules/dealEngineModule.h"

#include "../logger/deLogger.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/xmlparser/decXmlWriter.h"
#include "../common/xmlparser/decXmlDocument.h"
#include "../common/xmlparser/decXmlDocumentReference.h"
#include "../common/xmlparser/decXmlCharacterData.h"
#include "../common/xmlparser/decXmlElementTag.h"
#include "../common/xmlparser/decXmlAttValue.h"
#include "../common/xmlparser/decXmlVisitor.h"
#include "../common/xmlparser/decXmlParser.h"
#include "../common/exceptions.h"



// Class dealGameXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameXML::dealGameXML( deLogger *logger, const char *loggerSource ) :
dealBaseXML( logger, loggerSource ){
}

dealGameXML::~dealGameXML(){
}



// Management
///////////////

void dealGameXML::ReadFromFile( decBaseFileReader &reader, dealGame &game ){
	decXmlDocumentReference xmldoc;
	xmldoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( GetLogger() );
	
	parser.ParseXml( &reader, xmldoc );
	
	xmldoc->StripComments();
	xmldoc->CleanCharData();
	
	decXmlElementTag * const root = xmldoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "degame" ) != 0 ) DETHROW( deeInvalidParam );
	
	pReadGame( *root, game );
}



// Private Functions
//////////////////////

void dealGameXML::pReadGame( const decXmlElementTag &root, dealGame &game ){
	dealFileFormatList &fileFormatList = game.GetFileFormatList();
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "identifier" ) == 0 ){
			game.SetIdentifier( pGetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "title" ) == 0 ){
			game.SetTitle( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "subTitle" ) == 0 ){
			game.SetTitle( game.GetTitle() + decUnicodeString::NewFromUTF8( " - " ) + decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "description" ) == 0 ){
			game.SetDescription( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "icon" ) == 0 ){
			// todo
			
		}else if( strcmp( tag->GetName(), "creator" ) == 0 ){
			game.SetCreator( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "homepage" ) == 0 ){
			game.SetHomepage( pGetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "license" ) == 0 ){
			// deprecated
			
		}else if( strcmp( tag->GetName(), "gameDirectory" ) == 0 ){
			game.SetGameDirectory( pGetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "dataDirectory" ) == 0 ){
			game.SetDataDirectory( pGetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "scriptDirectory" ) == 0 ){
			game.SetScriptDirectory( pGetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "gameObject" ) == 0 ){
			game.SetGameObject( pGetCDataString( *tag ) );
			
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
			const dealEngineModule::eModuleTypes formatType = dealEngineModule::GetTypeFromString( typeName );
			if( formatType == dealEngineModule::emtUnknown ){
				pErrorUnknownValue( *tag, typeName );
				DETHROW( deeInvalidParam );
			}
			
			dealFileFormat *fileFormat = NULL;
			try{
				fileFormat = new dealFileFormat;
				fileFormat->SetType( formatType );
				fileFormat->SetPattern( pGetCDataString( *tag ) );
				fileFormatList.AddFormat( fileFormat );
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
