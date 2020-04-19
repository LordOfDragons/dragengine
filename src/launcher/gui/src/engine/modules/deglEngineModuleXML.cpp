/* 
 * Drag[en]gine GUI Launcher
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

#include "deglEngineModule.h"
#include "deglEngineModuleXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/file/decPath.h>
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



// Class deglEngineModuleXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEngineModuleXML::deglEngineModuleXML( deLogger *logger, const char *loggerSource ) : deglBaseXML( logger, loggerSource ){
}

deglEngineModuleXML::~deglEngineModuleXML(){
}



// Management
///////////////

void deglEngineModuleXML::ReadFromFile( const char *filename, decBaseFileReader &reader, deglEngineModule &module ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "module" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadModule( *root, module );
	
	decPath basePath;
	basePath.SetFromUnix( filename );
	basePath.RemoveLastComponent(); // module.xml
	basePath.RemoveLastComponent(); // version
	module.SetDirectoryName( basePath.GetLastComponent() );
}



// Private Functions
//////////////////////

void deglEngineModuleXML::pReadModule( const decXmlElementTag &root, deglEngineModule &module ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "name" ) == 0 ){
				module.SetName( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "description" ) == 0 ){
				module.SetDescription( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "author" ) == 0 ){
				module.SetAuthor( decUnicodeString::NewFromUTF8( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "version" ) == 0 ){
				module.SetVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "type" ) == 0 ){
				module.SetType( deModuleSystem::GetTypeFromString( pGetCDataString( *tag ) ) );
				
			}else if( strcmp( tag->GetName(), "pattern" ) == 0 ){
				module.SetPattern( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "homepage" ) == 0 ){
				// no interest in this tag
				
			}else if( strcmp( tag->GetName(), "library" ) == 0 ){
				pReadModuleLibrary( *tag, module );
				
			}else if( strcmp( tag->GetName(), "data" ) == 0 ){
				// deprecated
				
			}else if( strcmp( tag->GetName(), "fallback" ) == 0 ){
				module.SetIsFallback( true );
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

void deglEngineModuleXML::pReadModuleLibrary( const decXmlElementTag &root, deglEngineModule &module ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "file" ) == 0 ){
				module.SetLibFileName( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "size" ) == 0 ){
				module.SetLibFileSizeShould( pGetCDataInt( *tag ) );
				
			}else if( strcmp( tag->GetName(), "sha1" ) == 0 ){
				module.SetLibFileHashShould( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "entrypoint" ) == 0 ){
				module.SetLibFileEntryPoint( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "preloadLibrary" ) == 0 ){
				// we do not care about this
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}
