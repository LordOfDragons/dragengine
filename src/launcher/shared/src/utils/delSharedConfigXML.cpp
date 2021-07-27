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

#include "delSharedConfigXML.h"
#include "../game/profile/delGameProfile.h"
#include "../game/profile/delGPModule.h"
#include "../game/profile/delGPMParameter.h"
#include "../game/profile/delGPDisableModuleVersion.h"
#include "../game/profile/delGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/logger/deLogger.h>



// Class delSharedConfigXML
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delSharedConfigXML::delSharedConfigXML( deLogger* logger, const char* loggingSource ) :
delBaseXML( logger, loggingSource ){
}

delSharedConfigXML::~delSharedConfigXML(){
}



// Protected Functions
////////////////////////

void delSharedConfigXML::WriteProfile( decXmlWriter &writer, const delGameProfile &profile, const char *tagName ){
	writer.WriteOpeningTagStart( tagName );
	if( ! profile.GetName().IsEmpty() ){
		writer.WriteAttributeString( "name", profile.GetName().GetString() );
	}
	writer.WriteOpeningTagEnd();
	
	WriteProfileSystems( writer, profile );
	WriteProfileDisableModuleVersions( writer, profile );
	WriteProfileModules( writer, profile );
	
	if( ! profile.GetRunArguments().IsEmpty() ){
		writer.WriteDataTagString( "runArguments", profile.GetRunArguments() );
	}
	if( profile.GetReplaceRunArguments() ){
		writer.WriteDataTagInt( "replaceRunArguments", profile.GetReplaceRunArguments() ? 1 : 0 );
	}
	
	WriteProfileWindow( writer, profile );
	
	writer.WriteClosingTag( tagName, true );
}

void delSharedConfigXML::WriteProfileSystems( decXmlWriter &writer, const delGameProfile &profile ){
	writer.WriteOpeningTag( "systems", false, true );
	
	writer.WriteDataTagString( "graphic", profile.GetModuleGraphic() );
	if( ! profile.GetModuleGraphicVersion().IsEmpty() ){
		writer.WriteDataTagString( "graphicVersion", profile.GetModuleGraphicVersion() );
	}
	
	writer.WriteDataTagString( "input", profile.GetModuleInput() );
	if( ! profile.GetModuleInputVersion().IsEmpty() ){
		writer.WriteDataTagString( "inputVersion", profile.GetModuleInputVersion() );
	}
	
	writer.WriteDataTagString( "physics", profile.GetModulePhysics() );
	if( ! profile.GetModulePhysicsVersion().IsEmpty() ){
		writer.WriteDataTagString( "physicsVersion", profile.GetModulePhysicsVersion() );
	}
	
	writer.WriteDataTagString( "animator", profile.GetModuleAnimator() );
	if( ! profile.GetModuleAnimatorVersion().IsEmpty() ){
		writer.WriteDataTagString( "animatorVersion", profile.GetModuleAnimatorVersion() );
	}
	
	writer.WriteDataTagString( "ai", profile.GetModuleAI() );
	if( ! profile.GetModuleAIVersion().IsEmpty() ){
		writer.WriteDataTagString( "aiVersion", profile.GetModuleAIVersion() );
	}
	
	writer.WriteDataTagString( "crashRecovery", profile.GetModuleCrashRecovery() );
	if( ! profile.GetModuleCrashRecoveryVersion().IsEmpty() ){
		writer.WriteDataTagString( "crashRecoveryVersion", profile.GetModuleCrashRecoveryVersion() );
	}
	
	writer.WriteDataTagString( "audio", profile.GetModuleAudio() );
	if( ! profile.GetModuleAudioVersion().IsEmpty() ){
		writer.WriteDataTagString( "audioVersion", profile.GetModuleAudioVersion() );
	}
	
	writer.WriteDataTagString( "synthesizer", profile.GetModuleSynthesizer() );
	if( ! profile.GetModuleSynthesizerVersion().IsEmpty() ){
		writer.WriteDataTagString( "synthesizerVersion", profile.GetModuleSynthesizerVersion() );
	}
	
	writer.WriteDataTagString( "network", profile.GetModuleNetwork() );
	if( ! profile.GetModuleNetworkVersion().IsEmpty() ){
		writer.WriteDataTagString( "networkVersion", profile.GetModuleNetworkVersion() );
	}
	
	writer.WriteClosingTag( "systems", true );
}

void delSharedConfigXML::WriteProfileDisableModuleVersions( decXmlWriter &writer, const delGameProfile &profile ){
	const delGPDisableModuleVersionList &list = profile.GetDisableModuleVersionList();
	const int count = list.GetCount();
	int i;
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "disableModuleVersions", false, true );
	
	for( i=0; i<count; i++ ){
		const delGPDisableModuleVersion &version = *list.GetAt( i );
		writer.WriteOpeningTagStart( "disableModuleVersion" );
		writer.WriteAttributeString( "name", version.GetName() );
		writer.WriteAttributeString( "version", version.GetVersion() );
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteClosingTag( "disableModuleVersions", true );
}

void delSharedConfigXML::WriteProfileModules( decXmlWriter &writer, const delGameProfile &profile ){
	const delGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetCount();
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "modules", false, true );
	
	for( i=0; i<count; i++ ){
		WriteProfileModule( writer, *moduleList.GetAt ( i ) );
	}
	
	writer.WriteClosingTag( "modules", true );
}

void delSharedConfigXML::WriteProfileModule( decXmlWriter &writer, const delGPModule &module ){
	writer.WriteOpeningTagStart( "module" );
	writer.WriteAttributeString( "name", module.GetName() );
	writer.WriteOpeningTagEnd();
	
	WriteProfileModuleParameters( writer, module );
	
	writer.WriteClosingTag( "module", true );
}

void delSharedConfigXML::WriteProfileModuleParameters( decXmlWriter &writer, const delGPModule &module ){
	const delGPMParameterList &parametersList = module.GetParameters();
	int i, count = parametersList.GetCount();
	
	writer.WriteOpeningTag( "parameters", false, true );
	
	for( i=0; i<count; i++ ){
		const delGPMParameter &parameters = *parametersList.GetAt ( i );
		
		writer.WriteOpeningTagStart( "parameter" );
		writer.WriteAttributeString( "name", parameters.GetName() );
		writer.WriteOpeningTagEnd( false, false );
		writer.WriteTextString( parameters.GetValue() );
		writer.WriteClosingTag( "parameter", false );
	}
	
	writer.WriteClosingTag( "parameters", true );
}

void delSharedConfigXML::WriteProfileWindow( decXmlWriter &writer, const delGameProfile &profile ){
	writer.WriteOpeningTag( "window", false, true );
	
	writer.WriteDataTagInt( "fullScreen", profile.GetFullScreen() ? 1 : 0 );
	writer.WriteDataTagInt( "width", profile.GetWidth() );
	writer.WriteDataTagInt( "height", profile.GetHeight() );
	
	writer.WriteClosingTag( "window", true );
}



void delSharedConfigXML::ReadProfile( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	if( HasAttribute( root, "name" ) ){
		profile.SetName( GetAttributeString( root, "name" ) );
	}
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "systems" ){
			ReadProfileSystems( *tag, profile );
			
		}else if( tagName == "disableModuleVersions" ){
			ReadProfileDisableModuleVersions( *tag, profile );
			
		}else if( tagName == "modules" ){
			ReadProfileModules( *tag, profile );
			
		}else if( tagName == "runArguments" ){
			profile.SetRunArguments( GetCDataString( *tag ) );
			
		}else if( tagName == "replaceRunArguments" ){
			profile.SetReplaceRunArguments( GetCDataBool( *tag ) );
			
		}else if( tagName == "window" ){
			ReadProfileWindow( *tag, profile );
		}
	}
}

void delSharedConfigXML::ReadProfileSystems( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "graphic" ){
			profile.SetModuleGraphic( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "graphicVersion" ){
			profile.SetModuleGraphicVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "input" ){
			profile.SetModuleInput( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "inputVersion" ){
			profile.SetModuleInputVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "physics" ){
			profile.SetModulePhysics( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "physicsVersion" ){
			profile.SetModulePhysicsVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "animator" ){
			profile.SetModuleAnimator( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "animatorVersion" ){
			profile.SetModuleAnimatorVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "ai" ){
			profile.SetModuleAI( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "aiVersion" ){
			profile.SetModuleAIVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "crashRecovery" ){
			profile.SetModuleCrashRecovery( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "crashRecoveryVersion" ){
			profile.SetModuleCrashRecoveryVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "audio" ){
			profile.SetModuleAudio( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "audioVersion" ){
			profile.SetModuleAudioVersion( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "network" ){
			profile.SetModuleNetwork( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "synthesizer" ){
			profile.SetModuleSynthesizer( GetCDataString( *tag ) );
			
		}else if( tag->GetName() == "networkVersion" ){
			profile.SetModuleNetworkVersion( GetCDataString( *tag ) );
		}
	}
}

void delSharedConfigXML::ReadProfileDisableModuleVersions( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "disableModuleVersion" ){
			profile.GetDisableModuleVersionList().Add( delGPDisableModuleVersion::Ref::New(
				new delGPDisableModuleVersion( GetAttributeString( *tag, "name" ),
					GetAttributeString( *tag, "version" ) ) ) );
		}
	}
}

void delSharedConfigXML::ReadProfileModules( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "module" ){
			ReadProfileModule( *tag, profile );
		}
	}
}

void delSharedConfigXML::ReadProfileModule( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	const delGPModule::Ref module( delGPModule::Ref::New(
		new delGPModule( GetAttributeString( root, "name" ) ) ) );
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "parameters" ){
			ReadProfileModuleParameters( *tag, module );
		}
	}
	
	profile.GetModuleList().Add ( module );
}

void delSharedConfigXML::ReadProfileModuleParameters( const decXmlElementTag &root, delGPModule &module ){
	delGPMParameterList &parametersList = module.GetParameters();
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "parameter" ){
			parametersList.Add ( delGPMParameter::Ref::New( new delGPMParameter(
				GetAttributeString( *tag, "name" ), GetCDataString( *tag ) ) ) );
		}
	}
}

void delSharedConfigXML::ReadProfileWindow( const decXmlElementTag &root, delGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "fullScreen" ){
			profile.SetFullScreen( GetCDataBool( *tag ) );
			
		}else if( tagName == "width" ){
			profile.SetWidth( GetCDataInt( *tag ) );
			
		}else if( tagName == "height" ){
			profile.SetHeight( GetCDataInt( *tag ) );
		}
	}
}
