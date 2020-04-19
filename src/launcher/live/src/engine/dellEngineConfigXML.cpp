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

#include "dellEngine.h"
#include "dellEngineConfigXML.h"
#include "../dellLauncher.h"
#include "../game/dellGameManager.h"
#include "../game/profile/dellGameProfile.h"
#include "../game/profile/dellGPModule.h"
#include "../game/profile/dellGPMParameter.h"
#include "../game/profile/dellGPDisableModuleVersion.h"
#include "../game/profile/dellGPDisableModuleVersionList.h"

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



// Class dellEngineConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEngineConfigXML::dellEngineConfigXML( deLogger *logger, const char *loggerSource ) :
dellSharedConfigXML( logger, loggerSource ){
}

dellEngineConfigXML::~dellEngineConfigXML(){
}



// Management
///////////////

void dellEngineConfigXML::ReadFromFile( decBaseFileReader &reader, dellLauncher &launcher ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( launcher.GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "launcherConfig" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, launcher );
}

void dellEngineConfigXML::WriteToFile( decBaseFileWriter &writer, const dellLauncher &launcher ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, launcher );
}



// Private Functions
//////////////////////

void dellEngineConfigXML::pWriteConfig( decXmlWriter &writer, const dellLauncher &launcher ){
	const dellGameManager &gameManager = launcher.GetGameManager();
	
	writer.WriteOpeningTag( "launcherConfig", false, true );
	
	pWriteProfiles( writer, launcher );
	
	if( gameManager.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", gameManager.GetActiveProfile()->GetName().GetString() );
	}
	
	writer.WriteClosingTag( "launcherConfig", true );
}

void dellEngineConfigXML::pWriteProfiles( decXmlWriter &writer, const dellLauncher &launcher ){
	const dellGameManager &gameManager = launcher.GetGameManager();
	const dellGameProfileList &profileList = gameManager.GetProfileList();
	int i, count = profileList.GetCount();
	
	writer.WriteOpeningTag( "profiles", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfile( writer, *profileList.GetAt( i ), "profile" );
	}
	
	writer.WriteClosingTag( "profiles", true );
}



void dellEngineConfigXML::pReadConfig( const decXmlElementTag &root, dellLauncher &launcher ){
	dellGameManager &gameManager = launcher.GetGameManager();
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	const char *profileName;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "profiles" ) == 0 ){
				pReadProfiles( *tag, launcher );
				
			}else if( strcmp( tag->GetName(), "activeProfile" ) == 0 ){
				profileName = pGetCDataString( *tag );
				
				if( strlen( profileName ) == 0 ){
					gameManager.SetActiveProfile( NULL );
					
				}else{
					gameManager.SetActiveProfile( gameManager.GetProfileList().GetNamed( profileName ) );
					if( ! gameManager.GetActiveProfile() ){
						GetLogger()->LogWarnFormat( GetLoggerSource().GetString(),
							"%s(%i:%i): Profile '%s' does not exist", tag->GetName().GetString(),
							tag->GetLineNumber(), tag->GetPositionNumber(), profileName );
					}
				}
				
			}else{
				pErrorUnknownTag( root, *tag );
			}
		}
	}
}

void dellEngineConfigXML::pReadProfiles( const decXmlElementTag &root, dellLauncher &launcher ){
	dellGameManager &gameManager = launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "profile" ){
			deObjectReference refProfile;
			refProfile.TakeOver( new dellGameProfile );
			dellGameProfile &profile = ( dellGameProfile& )( deObject& )refProfile;
			
			pReadProfile( *tag, profile );
			
			if( ! profile.GetName().IsEmpty()
			&& ! gameManager.GetProfileList().HasNamed( profile.GetName() ) ){
				gameManager.GetProfileList().Add( &profile );
			}
			
		}else{
			pErrorUnknownTag( root, *tag );
		}
	}
}
