/* 
 * Drag[en]gine IGDE Project Editor
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

#include "projLauncherEngineConfig.h"
#include "../testrunner/projTestRunner.h"
#include "../testrunner/profile/projTRProfile.h"
#include "../testrunner/profile/projTRPParameter.h"
#include "../gui/projWindowMain.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
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



// Class projLauncherEngineConfig
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projLauncherEngineConfig::projLauncherEngineConfig( deLogger *logger, const char *loggerSource ) :
igdeBaseXML( logger, loggerSource ){
}

projLauncherEngineConfig::~projLauncherEngineConfig(){
}



// Management
///////////////

void projLauncherEngineConfig::ReadFromFile(
decBaseFileReader &reader, projTestRunner &testRunner ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser parser( testRunner.GetWindowMain().GetLogger() );
	
	parser.ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "launcherConfig" ){
		DETHROW( deeInvalidParam );
	}
	
	pReadConfig( *root, testRunner );
}



// Private Functions
//////////////////////

void projLauncherEngineConfig::pReadConfig(
const decXmlElementTag &root, projTestRunner &testRunner ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "profiles" ){
			pReadProfiles( *tag, testRunner );
			
		}else if( tagName == "activeProfile" ){
			testRunner.SetDefaultLauncherProfileName( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfiles(
const decXmlElementTag &root, projTestRunner &testRunner ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "profile" ){
			pReadProfile( *tag, testRunner );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfile(
const decXmlElementTag &root, projTestRunner &testRunner ){
	const int elementCount = root.GetElementCount();
	projTRProfile *profile = NULL;
	int i;
	
	try{
		profile = new projTRProfile;
		profile->SetName( GetAttributeString( root, "name" ) );
		
		for( i=0; i<elementCount; i++ ){
			const decXmlElementTag * const tag = root.GetElementIfTag( i );
			if( ! tag ){
				continue;
			}
			
			const decString &tagName = tag->GetName();
			
			if( tagName == "systems" ){
				pReadProfileSystems( *tag, *profile );
				
			}else if( tagName == "disableModuleVersions" ){
				profile->GetDisableModuleVersions().SetAt(
					GetAttributeString( *tag, "name" ),
					GetAttributeString( *tag, "version" ) );
				
			}else if( tagName == "modules" ){
				pReadProfileModules( *tag, *profile );
				
			}else if( tagName == "runArguments" ){
				profile->SetRunArguments( GetCDataString( *tag ) );
				
			}else if( tagName == "replaceRunArguments" ){
				profile->SetReplaceRunArguments( GetCDataInt( *tag ) != 0 );
				
			}else if( tagName == "window" ){
				pReadProfileWindow( *tag, *profile );
				
			}else{
				LogWarnUnknownTag( root, *tag );
			}
		}
		
		testRunner.GetLauncherProfiles().Add( profile );
		profile->FreeReference();
		
	}catch( const deException & ){
		if( profile ){
			profile->FreeReference();
		}
		throw;
	}
}

void projLauncherEngineConfig::pReadProfileSystems(
const decXmlElementTag &root, projTRProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "graphic" ){
			profile.SetModuleGraphic( GetCDataString( *tag ) );
			
		}else if( tagName == "input" ){
			profile.SetModuleInput( GetCDataString( *tag ) );
			
		}else if( tagName == "physics" ){
			profile.SetModulePhysics( GetCDataString( *tag ) );
			
		}else if( tagName == "animator" ){
			profile.SetModuleAnimator( GetCDataString( *tag ) );
			
		}else if( tagName == "ai" ){
			profile.SetModuleAI( GetCDataString( *tag ) );
			
		}else if( tagName == "crashRecovery" ){
			profile.SetModuleCrashRecovery( GetCDataString( *tag ) );
			
		}else if( tagName == "audio" ){
			profile.SetModuleAudio( GetCDataString( *tag ) );
			
		}else if( tagName == "synthesizer" ){
			profile.SetModuleSynthesizer( GetCDataString( *tag ) );
			
		}else if( tagName == "network" ){
			profile.SetModuleNetwork( GetCDataString( *tag ) );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfileModules(
const decXmlElementTag &root, projTRProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "module" ){
			pReadProfileModule( *tag, profile );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfileModule(
const decXmlElementTag &root, projTRProfile &profile ){
	const decString module( GetAttributeString( root, "name" ) );
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "parameters" ){
			pReadProfileModuleParameters( *tag, profile, module );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfileModuleParameters(
const decXmlElementTag &root, projTRProfile &profile, const char *module ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "parameter" ){
			projTRPParameter *parameter = NULL;
			
			try{
				parameter = new projTRPParameter;
				parameter->SetModule( module );
				parameter->SetName( GetAttributeString( *tag, "name" ) );
				parameter->SetValue( GetCDataString( *tag ) );
				
				profile.GetParameters().Add( parameter );
				parameter->FreeReference();
				
			}catch( const deException & ){
				if( parameter ){
					parameter->FreeReference();
				}
				throw;
			}
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}

void projLauncherEngineConfig::pReadProfileWindow(
const decXmlElementTag &root, projTRProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		
		if( tagName == "width" ){
			profile.SetWidth( GetCDataInt( *tag ) );
			
		}else if( tagName == "height" ){
			profile.SetHeight( GetCDataInt( *tag ) );
			
		}else if( tagName == "fullScreen" ){
			profile.SetFullScreen( GetCDataInt( *tag ) != 0 );
			
		}else{
			LogWarnUnknownTag( root, *tag );
		}
	}
}
