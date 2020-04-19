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

#include "deglSharedConfigXML.h"
#include "game/profile/deglGameProfile.h"
#include "game/profile/deglGPModule.h"
#include "game/profile/deglGPMParameter.h"
#include "game/profile/deglGPDisableModuleVersion.h"
#include "game/profile/deglGPDisableModuleVersionList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>



// Class deglSharedConfigXML
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglSharedConfigXML::deglSharedConfigXML( deLogger* logger, const char* loggingSource ) :
deglBaseXML( logger, loggingSource ){
}

deglSharedConfigXML::~deglSharedConfigXML(){
}



// Protected Functions
////////////////////////

void deglSharedConfigXML::pWriteProfile( decXmlWriter &writer, const deglGameProfile &profile, const char *tagName ){
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

void deglSharedConfigXML::pWriteProfileSystems( decXmlWriter &writer, const deglGameProfile &profile ){
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

void deglSharedConfigXML::pWriteProfileDisableModuleVersions( decXmlWriter &writer, const deglGameProfile &profile ){
	const deglGPDisableModuleVersionList &list = profile.GetDisableModuleVersionList();
	const int count = list.GetCount();
	int i;
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "disableModuleVersions", false, true );
	
	for( i=0; i<count; i++ ){
		const deglGPDisableModuleVersion &version = *list.GetAt( i );
		writer.WriteOpeningTagStart( "disableModuleVersion" );
		writer.WriteAttributeString( "name", version.GetName() );
		writer.WriteAttributeString( "version", version.GetVersion() );
		writer.WriteOpeningTagEnd( true );
	}
	
	writer.WriteClosingTag( "disableModuleVersions", true );
}

void deglSharedConfigXML::pWriteProfileModules( decXmlWriter &writer, const deglGameProfile &profile ){
	const deglGPModuleList &moduleList = profile.GetModuleList();
	int i, count = moduleList.GetModuleCount();
	
	if( count == 0 ){
		return;
	}
	
	writer.WriteOpeningTag( "modules", false, true );
	
	for( i=0; i<count; i++ ){
		pWriteProfileModule( writer, *moduleList.GetModuleAt( i ) );
	}
	
	writer.WriteClosingTag( "modules", true );
}

void deglSharedConfigXML::pWriteProfileModule( decXmlWriter &writer, const deglGPModule &module ){
	writer.WriteOpeningTagStart( "module" );
	writer.WriteAttributeString( "name", module.GetName() );
	writer.WriteOpeningTagEnd();
	
	pWriteProfileModuleParameters( writer, module );
	
	writer.WriteClosingTag( "module", true );
}

void deglSharedConfigXML::pWriteProfileModuleParameters( decXmlWriter &writer, const deglGPModule &module ){
	const deglGPMParameterList &parametersList = module.GetParameterList();
	int i, count = parametersList.GetParameterCount();
	
	writer.WriteOpeningTag( "parameters", false, true );
	
	for( i=0; i<count; i++ ){
		const deglGPMParameter &parameters = *parametersList.GetParameterAt( i );
		
		writer.WriteOpeningTagStart( "parameter" );
		writer.WriteAttributeString( "name", parameters.GetName() );
		writer.WriteOpeningTagEnd( false, false );
		writer.WriteTextString( parameters.GetValue() );
		writer.WriteClosingTag( "parameter", false );
	}
	
	writer.WriteClosingTag( "parameters", true );
}

void deglSharedConfigXML::pWriteProfileWindow( decXmlWriter &writer, const deglGameProfile &profile ){
	writer.WriteOpeningTag( "window", false, true );
	
	writer.WriteDataTagInt( "fullScreen", profile.GetFullScreen() ? 1 : 0 );
	writer.WriteDataTagInt( "width", profile.GetWidth() );
	writer.WriteDataTagInt( "height", profile.GetHeight() );
	
	writer.WriteClosingTag( "window", true );
}



void deglSharedConfigXML::pReadProfile( const decXmlElementTag &root, deglGameProfile &profile ){
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
			profile.SetReplaceRunArguments( GetCDataBool( *tag ) );
			
		}else if( tagName == "window" ){
			pReadProfileWindow( *tag, profile );
		}
	}
}

void deglSharedConfigXML::pReadProfileSystems( const decXmlElementTag &root, deglGameProfile &profile ){
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

void deglSharedConfigXML::pReadProfileDisableModuleVersions( const decXmlElementTag &root, deglGameProfile &profile ){
	const int elementCount = root.GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		
		if( tag ){
			if( strcmp( tag->GetName(), "disableModuleVersion" ) == 0 ){
				deglGPDisableModuleVersion *entry = NULL;
				const char * const name = pGetAttributeString( *tag, "name" );
				const char * const version = pGetAttributeString( *tag, "version" );
				
				try{
					entry = new deglGPDisableModuleVersion( name, version );
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

void deglSharedConfigXML::pReadProfileModules( const decXmlElementTag &root, deglGameProfile &profile ){
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

void deglSharedConfigXML::pReadProfileModule( const decXmlElementTag &root, deglGameProfile &profile ){
	int e, elementCount = root.GetElementCount();
	deglGPModule *module = NULL;
	const decXmlElementTag *tag;
	decString moduleName;
	
	moduleName = pGetAttributeString( root, "name" );
	
	try{
		module = new deglGPModule;
		if( ! module ) DETHROW( deeOutOfMemory );
		
		module->SetName( moduleName );
		
		for( e=0; e<elementCount; e++ ){
			tag = root.GetElementIfTag( e );
			
			if( tag ){
				if( strcmp( tag->GetName(), "parameters" ) == 0 ){
					pReadProfileModuleParameters( *tag, *module );
				}
			}
		}
		
		profile.GetModuleList().AddModule( module );
		
	}catch( const deException & ){
		if( module ) module->FreeReference();;
		throw;
	}
}

void deglSharedConfigXML::pReadProfileModuleParameters( const decXmlElementTag &root, deglGPModule &module ){
	deglGPMParameterList &parametersList = module.GetParameterList();
	int e, elementCount = root.GetElementCount();
	const decXmlElementTag *tag;
	deglGPMParameter *parameters;
	
	for( e=0; e<elementCount; e++ ){
		tag = root.GetElementIfTag( e );
		
		if( tag ){
			if( strcmp( tag->GetName(), "parameter" ) == 0 ){
				parameters = NULL;
				
				try{
					parameters = new deglGPMParameter;
					if( ! parameters ) DETHROW( deeOutOfMemory );
					
					parameters->SetName( pGetAttributeString( *tag, "name" ) );
					parameters->SetValue( pGetCDataString( *tag ) );
					
					parametersList.AddParameter( parameters );
					
				}catch( const deException & ){
					if( parameters ) parameters->FreeReference();
					throw;
				}
			}
		}
	}
}

void deglSharedConfigXML::pReadProfileWindow( const decXmlElementTag &root, deglGameProfile &profile ){
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
			profile.SetWidth( pGetCDataInt( *tag ) );
			
		}else if( tagName == "height" ){
			profile.SetHeight( pGetCDataInt( *tag ) );
		}
	}
}
