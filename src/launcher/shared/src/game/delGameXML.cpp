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
#include "../delLauncher.h"

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
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::New( new decXmlDocument ) );
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "degame" ){
		DETHROW_INFO( deeInvalidParam, "missing root tag 'degame'" );
	}
	
	pReadGame( *root, game );
}



// Private Functions
//////////////////////

void delGameXML::pReadGame( const decXmlElementTag &root, delGame &game ){
	delFileFormatList &fileFormats = game.GetFileFormats();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "identifier" ){
			game.SetIdentifier( decUuid( GetCDataString( *tag ), false ) );
			
		}else if( tag->GetName() == "aliasIdentifier" ){
			game.SetAliasIdentifier( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "title" ){
			game.SetTitle( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "subTitle" ){
			game.SetTitle( game.GetTitle() + decUnicodeString::NewFromUTF8( " - " )
				+ decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "description" ){
			game.SetDescription( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "icon" ){
			game.GetIcons().Add( delGameIcon::Ref::New( game.GetLauncher().CreateGameIcon(
				GetAttributeInt( *tag, "size" ), GetCDataString( *tag ) ) ) );
			
		}else if( tag->GetName() == "creator" ){
			game.SetCreator( decUnicodeString::NewFromUTF8( GetCDataString( *tag ) ) );
			
		}else if( tag->GetName() == "homepage" ){
			game.SetHomepage( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "license" ){
			// deprecated
			
		}else if( tag->GetName() == "gameDirectory" ){
			game.SetGameDirectory( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "dataDirectory" ){
			game.SetDataDirectory( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "scriptDirectory" ){
			game.SetScriptDirectory( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "gameObject" ){
			game.SetGameObject( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "pathConfig" ){
			game.SetPathConfig( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "pathCapture" ){
			game.SetPathCapture( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "scriptModule" ){
			game.SetScriptModule( GetCDataString( *tag ) );
			if( HasAttribute( *tag, "version" ) ){
				game.SetScriptModuleVersion( GetAttributeString( *tag, "version" ) );
			}
			
		}else if( tag->GetName() == "windowSize" ){
			game.SetWindowSize( decPoint( GetAttributeInt( *tag, "x" ), GetAttributeInt( *tag, "y" ) ) );
			
		}else if( tag->GetName() == "requireFormat" ){
			const decString typeName( GetAttributeString( *tag, "type" ) );
			const deModuleSystem::eModuleTypes formatType = deModuleSystem::GetTypeFromString( typeName );
			if( formatType == deModuleSystem::emtUnknown ){
				ErrorUnknownValue( *tag, typeName );
				DETHROW_INFO( deeInvalidParam, "invalid tag value" );
			}
			
			fileFormats.Add ( delFileFormat::Ref::New( new delFileFormat( formatType, GetCDataString( *tag ) ) ) );
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
	
	if( ! game.GetIdentifier() ){
		ErrorMissingTag( root, "identifier" );
		DETHROW_INFO( deeInvalidParam, "missing tag" );
	}
}
