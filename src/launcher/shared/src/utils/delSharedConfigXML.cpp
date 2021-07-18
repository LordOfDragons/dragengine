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

void delSharedConfigXML::pWriteProfile( decXmlWriter &writer, const delGameProfile &profile, const char *tagName ){
	writer.WriteOpeningTagStart( tagName );
	if( ! profile.GetName().IsEmpty() ){
		writer.WriteAttributeString( "name", profile.GetName().GetString() );
	}
	writer.WriteOpeningTagEnd();
	
	pWriteProfileSystems( writer, profile );
	pWriteProfileDisableModuleVersions( writer, profile );
	pWriteProfileModules( writer, profile );
	
	if( ! profile.GetRunArguments().IsEmpty() ){
		writer.WriteDataTagString( "runArguments", profile.GetRunArguments() );
	}
	if( profile.GetReplaceRunArguments() ){
		writer.WriteDataTagInt( "replaceRunArguments", profile.GetReplaceRunArguments() ? 1 : 0 );
	}
	
	pWriteProfileWindow( writer, profile );
	
	writer.WriteClosingTag( tagName, true );
}

void delSharedConfigXML::pWriteProfileSystems( decXmlWriter &writer, const delGameProfile &profile ){
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

void delSharedConfigXML::pWriteProfileDisableModuleVersions( decXmlWriter &writer, const delGameProfile &profile ){
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

void delSharedConfigXML::pWriteProfileModules( decXmlWriter &writer, const delGameProfile &profile ){
	const delGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetCount();
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "modules", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfileModule( writer, *moduleList.GetAt ( i ) );
	}
	
	writer.WriteClosingTag( "modules", true );
}

void delSharedConfigXML::pWriteProfileModule( decXmlWriter &writer, const delGPModule &module ){
	writer.WriteOpeningTagStart( "module" );
	writer.WriteAttributeString( "name", module.GetName() );
	writer.WriteOpeningTagEnd();
	
	pWriteProfileModuleParameters( writer, module );
	
	writer.WriteClosingTag( "module", true );
}

void delSharedConfigXML::pWriteProfileModuleParameters( decXmlWriter &writer, const delGPModule &module ){
	const delGPMParameterList &parametersList = module.GetParameterList();
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

void delSharedConfigXML::pWriteProfileWindow( decXmlWriter &writer, const delGameProfile &profile ){
	writer.WriteOpeningTag( "window", false, true );
	
	writer.WriteDataTagInt( "fullScreen", profile.GetFullScreen() ? 1 : 0 );
	writer.WriteDataTagInt( "width", profile.GetWidth() );
	writer.WriteDataTagInt( "height", profile.GetHeight() );
	
	writer.WriteClosingTag( "window", true );
}



void delSharedConfigXML::pReadProfile( const decXmlElementTag &root, delGameProfile &profile ){
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
			pReadProfileSystems( *tag, profile );
			
		}else if( tagName == "disableModuleVersions" ){
			pReadProfileDisableModuleVersions( *tag, profile );
			
		}else if( tagName == "modules" ){
			pReadProfileModules( *tag, profile );
			
		}else if( tagName == "runArguments" ){
			profile.SetRunArguments( GetCDataString( *tag ) );
			
		}else if( tagName == "replaceRunArguments" ){
			profile.SetReplaceRunArguments( GetCDataBool( *tag ) );
			
		}else if( tagName == "window" ){
			pReadProfileWindow( *tag, profile );
		}
	}
}

void delSharedConfigXML::pReadProfileSystems( const decXmlElementTag &root, delGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "graphic" ) == 0 ){
				profile.SetModuleGraphic( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "graphicVersion" ) == 0 ){
				profile.SetModuleGraphicVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "input" ) == 0 ){
				profile.SetModuleInput( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "inputVersion" ) == 0 ){
				profile.SetModuleInputVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "physics" ) == 0 ){
				profile.SetModulePhysics( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "physicsVersion" ) == 0 ){
				profile.SetModulePhysicsVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "animator" ) == 0 ){
				profile.SetModuleAnimator( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "animatorVersion" ) == 0 ){
				profile.SetModuleAnimatorVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "ai" ) == 0 ){
				profile.SetModuleAI( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "aiVersion" ) == 0 ){
				profile.SetModuleAIVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "crashRecovery" ) == 0 ){
				profile.SetModuleCrashRecovery( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "crashRecoveryVersion" ) == 0 ){
				profile.SetModuleCrashRecoveryVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "audio" ) == 0 ){
				profile.SetModuleAudio( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "audioVersion" ) == 0 ){
				profile.SetModuleAudioVersion( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "network" ) == 0 ){
				profile.SetModuleNetwork( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "synthesizer" ) == 0 ){
				profile.SetModuleSynthesizer( GetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "networkVersion" ) == 0 ){
				profile.SetModuleNetworkVersion( GetCDataString( *tag ) );
			}
		}
	}
}

void delSharedConfigXML::pReadProfileDisableModuleVersions( const decXmlElementTag &root, delGameProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "disableModuleVersion" ){
			profile.GetDisableModuleVersionList().Add( delGPDisableModuleVersion::Ref::With(
				new delGPDisableModuleVersion( GetAttributeString( *tag, "name" ),
					GetAttributeString( *tag, "version" ) ) ) );
		}
	}
}

void delSharedConfigXML::pReadProfileModules( const decXmlElementTag &root, delGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "module" ) == 0 ){
				pReadProfileModule( *tag, profile );
			}
		}
	}
}

void delSharedConfigXML::pReadProfileModule( const decXmlElementTag &root, delGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	const delGPModule::Ref module( delGPModule::Ref::With(
		new delGPModule( GetAttributeString( root, "name" ) ) ) );
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( tag->GetName() == "parameters" ){
				pReadProfileModuleParameters( *tag, module );
			}
		}
	}
	
	profile.GetModuleList().Add ( module );
}

void delSharedConfigXML::pReadProfileModuleParameters( const decXmlElementTag &root, delGPModule &module ){
	delGPMParameterList &parametersList = module.GetParameterList();
	int e, elementCount = root.GetElementCount();
	
	for( e=0; e<elementCount; e++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( e );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "parameter" ){
			parametersList.Add ( delGPMParameter::Ref::With( new delGPMParameter(
				GetAttributeString( *tag, "name" ), GetCDataString( *tag ) ) ) );
		}
	}
}

void delSharedConfigXML::pReadProfileWindow( const decXmlElementTag &root, delGameProfile &profile ){
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
