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

#include "delEngine.h"
#include "delEngineConfigXML.h"
#include "../delLauncherSupport.h"
#include "../game/delGameManager.h"
#include "../game/profile/delGameProfile.h"
#include "../game/profile/delGPModule.h"
#include "../game/profile/delGPMParameter.h"
#include "../game/profile/delGPDisableModuleVersion.h"
#include "../game/profile/delGPDisableModuleVersionList.h"

#include <dragengine/logger/deLogger.h>
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



// Class delEngineConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineConfigXML::delEngineConfigXML( deLogger *logger, const char *loggerSource ) :
delSharedConfigXML( logger, loggerSource ){
}

delEngineConfigXML::~delEngineConfigXML(){
}



// Management
///////////////

void delEngineConfigXML::ReadFromFile( decBaseFileReader &reader, delLauncherSupport &launcher ){
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::With( new decXmlDocument ) );
	decXmlParser( launcher.GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "launcherConfig" ){
		DETHROW_INFO( deeInvalidParam, "missing root tag 'launcherConfig'" );
	}
	
	pReadConfig( *root, launcher );
}

void delEngineConfigXML::WriteToFile( decBaseFileWriter &writer, const delLauncherSupport &launcher ){
	decXmlWriter xmlWriter( &writer );
	xmlWriter.WriteXMLDeclaration();
	pWriteConfig( xmlWriter, launcher );
}



// Private Functions
//////////////////////

void delEngineConfigXML::pWriteConfig( decXmlWriter &writer, const delLauncherSupport &launcher ){
	const delGameManager &gameManager = launcher.GetGameManager();
	
	writer.WriteOpeningTag( "launcherConfig", false, true );
	
	pWriteProfiles( writer, launcher );
	
	if( gameManager.GetActiveProfile() ){
		writer.WriteDataTagString( "activeProfile", gameManager.GetActiveProfile()->GetName() );
	}
	
	writer.WriteClosingTag( "launcherConfig", true );
}

void delEngineConfigXML::pWriteProfiles( decXmlWriter &writer, const delLauncherSupport &launcher ){
	const delGameManager &gameManager = launcher.GetGameManager();
	const delGameProfileList &profileList = gameManager.GetProfileList();
	const int count = profileList.GetCount();
	int i;
	
	writer.WriteOpeningTag( "profiles", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfile( writer, *profileList.GetAt ( i ), "profile" );
	}
	
	writer.WriteClosingTag( "profiles", true );
}



void delEngineConfigXML::pReadConfig( const decXmlElementTag &root, delLauncherSupport &launcher ){
	delGameManager &gameManager = launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "profiles" ){
			pReadProfiles( *tag, launcher );
			
		}else if( tag->GetName() == "activeProfile" ){
			const decString &profileName = GetCDataString( *tag );
			
			if( profileName.IsEmpty() ){
				gameManager.SetActiveProfile( nullptr );
				
			}else{
				gameManager.SetActiveProfile( gameManager.GetProfileList().GetNamed( profileName ) );
				if( ! gameManager.GetActiveProfile() ){
					GetLogger()->LogWarnFormat( GetLoggerSource().GetString(),
						"%s(%i:%i): Profile '%s' does not exist", tag->GetName().GetString(),
						tag->GetLineNumber(), tag->GetPositionNumber(), profileName.GetString() );
				}
			}
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
}

void delEngineConfigXML::pReadProfiles( const decXmlElementTag &root, delLauncherSupport &launcher ){
	delGameManager &gameManager = launcher.GetGameManager();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "profile" ){
			delGameProfile::Ref profile( delGameProfile::Ref::With( new delGameProfile ) );
			pReadProfile( *tag, profile );
			
			if( ! profile->GetName().IsEmpty()
			&& ! gameManager.GetProfileList().HasNamed ( profile->GetName() ) ){
				gameManager.GetProfileList().Add ( profile );
			}
			
		}else{
			ErrorUnknownTag( root, *tag );
		}
	}
}
