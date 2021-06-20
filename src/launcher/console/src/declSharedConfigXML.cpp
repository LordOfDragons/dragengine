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

#include "declSharedConfigXML.h"
#include "game/profile/declGameProfile.h"
#include "game/profile/declGPModule.h"
#include "game/profile/declGPMParameter.h"
#include "game/profile/declGPDisableModuleVersion.h"
#include "game/profile/declGPDisableModuleVersionList.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/logger/deLogger.h>



// Class declSharedConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declSharedConfigXML::declSharedConfigXML( deLogger* logger, const char* loggingSource ) :
declBaseXML( logger, loggingSource ){
}

declSharedConfigXML::~declSharedConfigXML(){
}



// Protected Functions
////////////////////////

void declSharedConfigXML::pWriteProfile( decXmlWriter &writer, const declGameProfile &profile, const char *tagName ){
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

void declSharedConfigXML::pWriteProfileSystems( decXmlWriter &writer, const declGameProfile &profile ){
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

void declSharedConfigXML::pWriteProfileDisableModuleVersions( decXmlWriter &writer, const declGameProfile &profile ){
	const declGPDisableModuleVersionList &list = profile.GetDisableModuleVersionList();
	const int count = list.GetCount();
	int i;
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "disableModuleVersions", false, true );
	
	for( i=0; i<count; i++ ){
		const declGPDisableModuleVersion &version = *list.GetAt( i );
		writer.WriteOpeningTagStart( "disableModuleVersion" );
		writer.WriteAttributeString( "name", version.GetName() );
		writer.WriteAttributeString( "version", version.GetVersion() );
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteClosingTag( "disableModuleVersions", true );
}

void declSharedConfigXML::pWriteProfileModules( decXmlWriter &writer, const declGameProfile &profile ){
	const declGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetCount();
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "modules", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfileModule( writer, *moduleList.GetAt( i ) );
	}
	
	writer.WriteClosingTag( "modules", true );
}

void declSharedConfigXML::pWriteProfileModule( decXmlWriter &writer, const declGPModule &module ){
	writer.WriteOpeningTagStart( "module" );
	writer.WriteAttributeString( "name", module.GetName() );
	writer.WriteOpeningTagEnd();
	
	pWriteProfileModuleParameters( writer, module );
	
	writer.WriteClosingTag( "module", true );
}

void declSharedConfigXML::pWriteProfileModuleParameters( decXmlWriter &writer, const declGPModule &module ){
	const declGPMParameterList &parametersList = module.GetParameterList();
	int i, count = parametersList.GetCount();
	
	writer.WriteOpeningTag( "parameters", false, true );
	
	for( i=0; i<count; i++ ){
		const declGPMParameter &parameters = *parametersList.GetAt( i );
		
		writer.WriteOpeningTagStart( "parameter" );
		writer.WriteAttributeString( "name", parameters.GetName() );
		writer.WriteOpeningTagEnd( false, false );
		writer.WriteTextString( parameters.GetValue() );
		writer.WriteClosingTag( "parameter", false );
	}
	
	writer.WriteClosingTag( "parameters", true );
}

void declSharedConfigXML::pWriteProfileWindow( decXmlWriter &writer, const declGameProfile &profile ){
	writer.WriteOpeningTag( "window", false, true );
	
	writer.WriteDataTagInt( "fullScreen", profile.GetFullScreen() ? 1 : 0 );
	writer.WriteDataTagInt( "width", profile.GetWidth() );
	writer.WriteDataTagInt( "height", profile.GetHeight() );
	
	writer.WriteClosingTag( "window", true );
}



void declSharedConfigXML::pReadProfile( const decXmlElementTag &root, declGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	if( pHasAttribute( root, "name" ) ){
		profile.SetName( pGetAttributeString( root, "name" ) );
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
			profile.SetRunArguments( pGetCDataString( *tag ) );
			
		}else if( tagName == "replaceRunArguments" ){
			profile.SetReplaceRunArguments( pGetCDataBool( *tag ) );
			
		}else if( tagName == "window" ){
			pReadProfileWindow( *tag, profile );
		}
	}
}

void declSharedConfigXML::pReadProfileSystems( const decXmlElementTag &root, declGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "graphic" ) == 0 ){
				profile.SetModuleGraphic( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "graphicVersion" ) == 0 ){
				profile.SetModuleGraphicVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "input" ) == 0 ){
				profile.SetModuleInput( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "inputVersion" ) == 0 ){
				profile.SetModuleInputVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "physics" ) == 0 ){
				profile.SetModulePhysics( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "physicsVersion" ) == 0 ){
				profile.SetModulePhysicsVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "animator" ) == 0 ){
				profile.SetModuleAnimator( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "animatorVersion" ) == 0 ){
				profile.SetModuleAnimatorVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "ai" ) == 0 ){
				profile.SetModuleAI( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "aiVersion" ) == 0 ){
				profile.SetModuleAIVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "crashRecovery" ) == 0 ){
				profile.SetModuleCrashRecovery( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "crashRecoveryVersion" ) == 0 ){
				profile.SetModuleCrashRecoveryVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "audio" ) == 0 ){
				profile.SetModuleAudio( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "audioVersion" ) == 0 ){
				profile.SetModuleAudioVersion( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "network" ) == 0 ){
				profile.SetModuleNetwork( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "synthesizer" ) == 0 ){
				profile.SetModuleSynthesizer( pGetCDataString( *tag ) );
				
			}else if( strcmp( tag->GetName(), "networkVersion" ) == 0 ){
				profile.SetModuleNetworkVersion( pGetCDataString( *tag ) );
			}
		}
	}
}

void declSharedConfigXML::pReadProfileDisableModuleVersions( const decXmlElementTag &root, declGameProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		
		if( tag ){
			if( strcmp( tag->GetName(), "disableModuleVersion" ) == 0 ){
				declGPDisableModuleVersion *entry = NULL;
				const char * const name = pGetAttributeString( *tag, "name" );
				const char * const version = pGetAttributeString( *tag, "version" );
				
				try{
					entry = new declGPDisableModuleVersion( name, version );
					profile.GetDisableModuleVersionList().Add( entry );
					entry->FreeReference();
					
				}catch( const deException & ){
					if( entry ){
						entry->FreeReference();
					}
					throw;
				}
			}
		}
	}
}

void declSharedConfigXML::pReadProfileModules( const decXmlElementTag &root, declGameProfile &profile ){
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

void declSharedConfigXML::pReadProfileModule( const decXmlElementTag &root, declGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	declGPModule *module = NULL;
	const decXmlElementTag *tag;
	decString moduleName;
	
	moduleName = pGetAttributeString( root, "name" );
	
	try{
		module = new declGPModule;
		module->SetName( moduleName );
		
		for( e=0; e<elementCount; e++ ){
			tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "parameters" ) == 0 ){
					pReadProfileModuleParameters( *tag, *module );
				}
			}
		}
		
		profile.GetModuleList().Add( module );
		module->FreeReference();
		module = NULL;
		
	}catch( const deException & ){
		if( module ) module->FreeReference();
		throw;
	}
}

void declSharedConfigXML::pReadProfileModuleParameters( const decXmlElementTag &root, declGPModule &module ){
	declGPMParameterList &parametersList = module.GetParameterList();
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	deObjectReference refParams;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( tag->GetName() == "parameter" ){
				
				refParams.TakeOver( new declGPMParameter );
				declGPMParameter * const parameters = ( declGPMParameter* )( deObject* )refParams;
				
				parameters->SetName( pGetAttributeString( *tag, "name" ) );
				parameters->SetValue( pGetCDataString( *tag ) );
				
				parametersList.Add( parameters );
			}
		}
	}
}

void declSharedConfigXML::pReadProfileWindow( const decXmlElementTag &root, declGameProfile &profile ){
	const int count = root.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "fullScreen" ){
			profile.SetFullScreen( pGetCDataBool( *tag ) );
			
		}else if( tagName == "width" ){
			profile.SetWidth( pGetCDataInt( *tag ) );
			
		}else if( tagName == "height" ){
			profile.SetHeight( pGetCDataInt( *tag ) );
		}
	}
}
