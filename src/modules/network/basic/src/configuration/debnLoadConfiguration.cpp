/* 
 * Drag[en]gine Basic Network Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "debnConfiguration.h"
#include "debnLoadConfiguration.h"
#include "../deNetworkBasic.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>


// Class debnLoadConfiguration
////////////////////////////////

// Constructor, destructor
////////////////////////////

debnLoadConfiguration::debnLoadConfiguration( deNetworkBasic &network ) :
pNetwork( network ){
}

debnLoadConfiguration::~debnLoadConfiguration(){
}



// Management
///////////////

void debnLoadConfiguration::LoadConfig( debnConfiguration &configuration ){
	deVirtualFileSystem& vfs = pNetwork.GetVFS();
	
	const decPath path( decPath::CreatePathUnix( "/config/config.xml" ) );
	if( ! vfs.ExistsFile( path ) ){
		pNetwork.LogInfo( "Configuration file 'config.xml' not found. Using defaults." );
		return;
	}
	
	pNetwork.LogInfo( "Loading configuration file 'config.xml'" );
	pLoadConfiguration( configuration, decBaseFileReader::Ref::New( vfs.OpenFileForReading( path ) ) );
}


// Private Functions
//////////////////////

void debnLoadConfiguration::pLoadConfiguration( debnConfiguration &configuration, decBaseFileReader &reader ){
	const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::New( new decXmlDocument ) );
	decXmlParser( pNetwork.GetGameEngine()->GetLogger() ).ParseXml( &reader, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || root->GetName() != "config" ){
		DETHROW_INFO( deeInvalidParam, "root tag not named 'config'" );
	}
	
	const int elementCount = root->GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		decXmlElementTag * const tag = pGetTagAt( root, i );
		if( ! tag ){
			continue;
		}
		
		const decString &tagName = tag->GetName();
		if( tagName == "property" ){
			const decString &name = pGetAttributeString( tag, "name" );
			
			if( name == "logLevel" ){
				DEASSERT_NOTNULL( tag->GetFirstData() )
				const decString &value = tag->GetFirstData()->GetData();
				if( value == "error" ){
					configuration.SetLogLevel( debnConfiguration::ellError );
					
				}else if( value == "warning" ){
					configuration.SetLogLevel( debnConfiguration::ellWarning );
					
				}else if( value == "info" ){
					configuration.SetLogLevel( debnConfiguration::ellInfo );
					
				}else if( value == "debug" ){
					configuration.SetLogLevel( debnConfiguration::ellDebug );
					
				}else{
					pNetwork.LogWarnFormat( "config.xml %s(%i:%i): Invalid value '%s' for property %s.",
						tagName.GetString(), tag->GetLineNumber(), tag->GetPositionNumber(),
						value.GetString(), name.GetString() );
				}
				
			}else if( name == "connectResendInterval" ){
				DEASSERT_NOTNULL( tag->GetFirstData() )
				configuration.SetConnectResendInterval( strtof( tag->GetFirstData()->GetData(), nullptr ) );
				
			}else if( name == "connectTimeout" ){
				DEASSERT_NOTNULL( tag->GetFirstData() )
				configuration.SetConnectTimeout( strtof( tag->GetFirstData()->GetData(), nullptr ) );
				
			}else if( name == "reliableResendInterval" ){
				DEASSERT_NOTNULL( tag->GetFirstData() )
				configuration.SetReliableResendInterval( strtof( tag->GetFirstData()->GetData(), nullptr ) );
				
			}else if( name == "reliableTimeout" ){
				DEASSERT_NOTNULL( tag->GetFirstData() )
				configuration.SetReliableTimeout( strtof( tag->GetFirstData()->GetData(), nullptr ) );
			}
			
		}else{
			pNetwork.LogWarnFormat( "config.xml %s(%i:%i): Unknown Tag %s.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}

decXmlElementTag *debnLoadConfiguration::pGetTagAt( decXmlElementTag *tag, int index ){
	decXmlElement * const element = tag->GetElementAt( index );
	if( element->CanCastToElementTag() ){
		return element->CastToElementTag();
		
	}else{
		return nullptr;
	}
}

decXmlAttValue *debnLoadConfiguration::pFindAttribute( decXmlElementTag *tag, const char *name ){
	int i;
	for( i=0; i<tag->GetElementCount(); i++ ){
		decXmlElement &element = *tag->GetElementAt( i );
		if( element.CanCastToAttValue() ){
			decXmlAttValue * const value = element.CastToAttValue();
			if( value->GetName() == name ){
				return value;
			}
		}
	}
	return nullptr;
}

const decString &debnLoadConfiguration::pGetAttributeString( decXmlElementTag *tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	if( value ){
		return value->GetValue();
	}
	
	pNetwork.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name, tag->GetName().GetString() );
	DETHROW_INFO( deeInvalidParam, "Missing attribute" );
}

int debnLoadConfiguration::pGetAttributeInt( decXmlElementTag *tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	if( value ){
		return (int)strtol( value->GetValue(), nullptr, 10 );
	}
	
	pNetwork.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name, tag->GetName().GetString() );
	DETHROW_INFO( deeInvalidParam, "Missing attribute" );
}

float debnLoadConfiguration::pGetAttributeFloat( decXmlElementTag *tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	if( value ){
		return strtof( value->GetValue(), nullptr );
	}
	
	pNetwork.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.", name, tag->GetName().GetString() );
	DETHROW_INFO( deeInvalidParam, "Missing attribute" );
}
