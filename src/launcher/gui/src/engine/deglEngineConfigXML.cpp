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

#include "deglEngine.h"
#include "deglEngineConfigXML.h"
#include "../deglLauncher.h"
#include "../game/deglGameManager.h"
#include "../game/profile/deglGameProfile.h"
#include "../game/profile/deglGPModule.h"
#include "../game/profile/deglGPMParameter.h"
#include "../game/profile/deglGPDisableModuleVersion.h"
#include "../game/profile/deglGPDisableModuleVersionList.h"

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



// Class deglEngineConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEngineConfigXML::deglEngineConfigXML( deLogger *logger, const char *loggerSource ) :
deglSharedConfigXML( logger, loggerSource ){
}

deglEngineConfigXML::~deglEngineConfigXML(){
}



// Management
///////////////

void deglEngineConfigXML::ReadFromFile( decBaseFileReader &reader, deglLauncher &launcher ){
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

void deglEngineConfigXML::WriteToFile( decBaseFileWriter &writer, const deglLauncher &launcher ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, launcher );
}



// Private Functions
//////////////////////

void deglEngineConfigXML::pWriteConfig( decXmlWriter &writer, const deglLauncher &launcher ){
	const deglGameManager &gameManager = *launcher.GetGameManager();
	
	writer.WriteOpeningTag( "launcherConfig", false, true );
	
	pWriteProfiles( writer, launcher );
	
	if( gameManager.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", gameManager.GetActiveProfile()->GetName().GetString() );
	}
	
	writer.WriteClosingTag( "launcherConfig", true );
}

void deglEngineConfigXML::pWriteProfiles( decXmlWriter &writer, const deglLauncher &launcher ){
	const deglGameManager &gameManager = *launcher.GetGameManager();
	const deglGameProfileList &profileList = gameManager.GetProfileList();
	int i, count = profileList.GetProfileCount();
	
	writer.WriteOpeningTag( "profiles", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfile( writer, *profileList.GetProfileAt( i ), "profile" );
	}
	
	writer.WriteClosingTag( "profiles", true );
}



void deglEngineConfigXML::pReadConfig( const decXmlElementTag &root, deglLauncher &launcher ){
	deglGameManager &gameManager = *launcher.GetGameManager();
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
					gameManager.SetActiveProfile( gameManager.GetProfileList().GetProfileNamed( profileName ) );
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

void deglEngineConfigXML::pReadProfiles( const decXmlElementTag &root, deglLauncher &launcher ){
	deglGameManager &gameManager = *launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "profile" ){
			deObjectReference refProfile;
			refProfile.TakeOver( new deglGameProfile );
			deglGameProfile &profile = ( deglGameProfile& )( deObject& )refProfile;
			
			pReadProfile( *tag, profile );
			
			if( ! profile.GetName().IsEmpty()
			&& ! gameManager.GetProfileList().HasProfileNamed( profile.GetName() ) ){
				gameManager.GetProfileList().AddProfile( &profile );
			}
			
		}else{
			pErrorUnknownTag( root, *tag );
		}
	}
}
