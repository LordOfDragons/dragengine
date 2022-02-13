/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delEngineModule.h"
#include "delEngineModuleXML.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/exceptions.h>



// Class delEngineModuleXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModuleXML::delEngineModuleXML( deLogger *logger, const char *loggerSource ) :
delBaseXML( logger, loggerSource ){
}

delEngineModuleXML::~delEngineModuleXML(){
}



// Management
///////////////

void delEngineModuleXML::ReadFromFile( const char *filename, decBaseFileReader &reader, delEngineModule &module ){
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::New( new decXmlDocument ) );
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "module" ){
		DETHROW_INFO( deeInvalidParam, "missing root tag 'module'" );
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

void delEngineModuleXML::pReadModule( const decXmlElementTag &root, delEngineModule &module ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "name" ){
			module.SetName( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "description" ){
			module.SetDescription( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "author" ){
			module.SetAuthor( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "version" ){
			module.SetVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "type" ){
			module.SetType( deModuleSystem::GetTypeFromString( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "pattern" ){
			module.SetPattern( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "homepage" ){
			// no interest in this tag
			
		}else if( tag->GetName() == "library" ){
			pReadModuleLibrary( *tag, module );
			
		}else if( tag->GetName() == "data" ){
			// deprecated
			
		}else if( tag->GetName() == "fallback" ){
			module.SetIsFallback( true );
			
		}else if( tag->GetName() == "priority" ){
			module.SetPriority( GetCDataInt( *tag ) );
			
		}else{
// 			ErrorUnknownTag( root, *tag );
		}
	}
}

void delEngineModuleXML::pReadModuleLibrary( const decXmlElementTag &root, delEngineModule &module ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "file" ){
			module.SetLibFileName( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "size" ){
			module.SetLibFileSizeShould( GetCDataInt( *tag ) );
			
		}else if( tag->GetName() == "sha1" ){
			module.SetLibFileHashShould( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "entrypoint" ){
			module.SetLibFileEntryPoint( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "preloadLibrary" ){
			// we do not care about this
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
}
