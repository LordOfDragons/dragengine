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
#include "dellGameManager.h"
#include "dellGameConfigXML.h"
#include "profile/dellGameProfile.h"
#include "profile/dellGPModule.h"
#include "profile/dellGPMParameter.h"
#include "profile/dellGPDisableModuleVersion.h"
#include "profile/dellGPDisableModuleVersionList.h"

#include <dragengine/logger/deLogger.h>
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



// Class dellGameConfigXML
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGameConfigXML::dellGameConfigXML( deLogger* logger, const char* loggingSource, dellGameManager &gameManager ) :
dellSharedConfigXML( logger, loggingSource ),
pGameManager( gameManager ){
}

dellGameConfigXML::~dellGameConfigXML(){
}



// Management
///////////////

void dellGameConfigXML::ReadFromFile( decBaseFileReader &reader, dellGame &game ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "gameConfig" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, game );
}

void dellGameConfigXML::WriteToFile( decBaseFileWriter &writer, const dellGame &game ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, game );
}



// Private Functions
//////////////////////

void dellGameConfigXML::pWriteConfig( decXmlWriter &writer, const dellGame &game ){
	writer.WriteOpeningTag( "gameConfig", false, true );
	
	if( game.GetCustomProfile() ){
		pWriteProfile( writer, *game.GetCustomProfile(), "customProfile" );
	}
	
	if( game.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", game.GetActiveProfile()->GetName() );
	}
	
	if( ! game.GetRunArguments().IsEmpty() ){
		writer.WriteDataTagString( "runArguments", game.GetRunArguments() );
	}
	
	writer.WriteDataTagBool( "useLatestPatch", game.GetUseLatestPatch() );
	if( game.GetUseCustomPatch() ){
		writer.WriteDataTagString( "useCustomPatch", game.GetUseCustomPatch().ToHexString( false ) );
	}
	
	writer.WriteClosingTag( "gameConfig", true );
}



void dellGameConfigXML::pReadConfig( const decXmlElementTag &root, dellGame &game ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "customProfile" ){
			deObjectReference refProfile;
			refProfile.TakeOver( new dellGameProfile );
			dellGameProfile &profile = ( dellGameProfile& )( deObject& )refProfile;
			
			pReadProfile( *tag, profile );
			profile.SetName( "" );
			
			game.SetCustomProfile( &profile );
			
		}else if( tagName == "activeProfile" ){
			const char * const name = pGetCDataString( *tag );
			if( ! name || ! name[ 0 ] ){
				game.SetActiveProfile( game.GetCustomProfile() );
				
			}else{
				game.SetActiveProfile( pGameManager.GetProfileList().GetNamed( name ) );
				if( ! game.GetActiveProfile() ){
					GetLogger()->LogWarnFormat( GetLoggerSource(), "%s(%i:%i): Profile '%s' does not exist",
						tag->GetName().GetString(), tag->GetLineNumber(), tag->GetPositionNumber(),
						name ? name : "" );
				}
			}
			
		}else if( tagName == "runArguments" ){
			game.SetRunArguments( pGetCDataString( *tag ) );
			
		}else if( tagName == "useLatestPatch" ){
			game.SetUseLatestPatch( pGetCDataBool( *tag ) );
			
		}else if( tagName == "useCustomPatch" ){
			const char * const value = pGetCDataString( *tag );
			if( value ){
				game.SetUseCustomPatch( value[ 0 ] ? decUuid( pGetCDataString( *tag ), false ) : decUuid() );
			}
		}
	}
}
