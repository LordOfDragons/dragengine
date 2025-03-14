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
