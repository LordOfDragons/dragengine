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

#include "delGame.h"
#include "delGameXML.h"
#include "icon/delGameIcon.h"
#include "fileformat/delFileFormat.h"
#include "fileformat/delFileFormatList.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
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



// Class delGameXML
//////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameXML::delGameXML( deLogger *logger, const char *loggerSource ) :
delBaseXML( logger, loggerSource ){
}

delGameXML::~delGameXML(){
}



// Management
///////////////

void delGameXML::ReadFromFile( decBaseFileReader &reader, delGame &game ){
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::With( new decXmlDocument ) );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "degame" ) != 0 ){
		DETHROW_INFO( deeInvalidParam, "missing tag 'degame'" );
	}
	
	pReadGame( *root, game );
}



// Private Functions
//////////////////////

void delGameXML::pReadGame( const decXmlElementTag &root, delGame &game ){
	delFileFormatList &fileFormats = game.GetFileFormats();
	int e, elementCount = root.GetElementCount();
	
	for( e=0; e<elementCount; e++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( e );
		if( ! tag ){
			continue;
		}
		
		if( strcmp( tag->GetName(), "identifier" ) == 0 ){
			game.SetIdentifier( decUuid( GetCDataString( *tag ), false ) );
			
		}else if( strcmp( tag->GetName(), "aliasIdentifier" ) == 0 ){
			game.SetAliasIdentifier( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "title" ) == 0 ){
			game.SetTitle( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "subTitle" ) == 0 ){
			game.SetTitle( game.GetTitle() + decUnicodeString::NewFromUTF8( " - " )
				+ decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "description" ) == 0 ){
			game.SetDescription( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "icon" ) == 0 ){
			game.GetIcons().Add( delGameIcon::Ref::With( new delGameIcon(
				GetAttributeInt( *tag, "size" ), GetCDataString( *tag ) ) ) );
			
		}else if( strcmp( tag->GetName(), "creator" ) == 0 ){
			game.SetCreator( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( strcmp( tag->GetName(), "homepage" ) == 0 ){
			game.SetHomepage( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "license" ) == 0 ){
			// deprecated
			
		}else if( strcmp( tag->GetName(), "gameDirectory" ) == 0 ){
			game.SetGameDirectory( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "dataDirectory" ) == 0 ){
			game.SetDataDirectory( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "scriptDirectory" ) == 0 ){
			game.SetScriptDirectory( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "gameObject" ) == 0 ){
			game.SetGameObject( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "pathConfig" ) == 0 ){
			game.SetPathConfig( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "pathCapture" ) == 0 ){
			game.SetPathCapture( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "scriptModule" ) == 0 ){
			game.SetScriptModule( GetCDataString( *tag ) );
			
		}else if( strcmp( tag->GetName(), "windowSize" ) == 0 ){
			game.SetWindowSize( decPoint( GetAttributeInt( *tag, "x" ), GetAttributeInt( *tag, "y" ) ) );
			
		}else if( strcmp( tag->GetName(), "requireFormat" ) == 0 ){
			const decString typeName( GetAttributeString( *tag, "type" ) );
			const deModuleSystem::eModuleTypes formatType = deModuleSystem::GetTypeFromString( typeName );
			if( formatType == deModuleSystem::emtUnknown ){
				ErrorUnknownValue( *tag, typeName );
				DETHROW_INFO( deeInvalidParam, "invalid tag value" );
			}
			
			fileFormats.Add ( delFileFormat::Ref::With(
				new delFileFormat( formatType, GetCDataString( *tag ) ) ) );
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
	
	if( ! game.GetIdentifier() ){
		ErrorMissingTag( root, "identifier" );
		DETHROW_INFO( deeInvalidParam, "missing tag 'identifier'" );
	}
}
