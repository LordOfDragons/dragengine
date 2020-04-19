/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalConfiguration.h"
#include "deoalLSConfiguration.h"
#include "../deAudioOpenAL.h"

#include <dragengine/deEngine.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlDocumentReference.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>



// Class deoalConfiguration
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalLSConfiguration::deoalLSConfiguration( deAudioOpenAL &oal, deoalConfiguration &config ) :
pOal( oal ),
pConfig( config ){
}

deoalLSConfiguration::~deoalLSConfiguration(){
}



// Management
///////////////

void deoalLSConfiguration::LoadConfig(){
	deVirtualFileSystem &vfs = pOal.GetVFS();
	const decPath path( decPath::CreatePathNative( "/config/openal.xml" ) );
	if( ! vfs.ExistsFile( path ) ){
		return;
	}
	
	decBaseFileReaderReference reader;
	reader.TakeOver( vfs.OpenFileForReading( path ) );
	pLoadConfig( reader );
}

void deoalLSConfiguration::SaveConfig(){
}



// Private Functions
//////////////////////

decXmlElementTag *deoalLSConfiguration::pGetTagAt( const decXmlElementTag &tag, int index ){
	decXmlElement &element = *tag.GetElementAt( index );
	
	if( element.CanCastToElementTag() ){
		return element.CastToElementTag();
		
	}else{
		return NULL;
	}
}

decXmlAttValue *deoalLSConfiguration::pFindAttribute( const decXmlElementTag &tag, const char *name ){
	const int count = tag.GetElementCount();
	int i;
	
	for( i=0; i<count; i++ ){
		decXmlElement &element = *tag.GetElementAt( i );
		
		if( element.CanCastToAttValue() ){
			decXmlAttValue * const value = element.CastToAttValue();
			
			if( value->GetName() == name ){
				return value;
			}
		}
	}
	
	return NULL;
}

const char *deoalLSConfiguration::pGetAttributeString( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return value->GetValue();
		
	}else{
		pOal.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

int deoalLSConfiguration::pGetAttributeInt( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return value->GetValue().ToInt();
		
	}else{
		pOal.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

float deoalLSConfiguration::pGetAttributeFloat( const decXmlElementTag &tag, const char *name ){
	const decXmlAttValue * const value = pFindAttribute( tag, name );
	
	if( value ){
		return value->GetValue().ToFloat();
		
	}else{
		pOal.LogErrorFormat( "Missing Attribute '%s' in tag '%s'.",
			name, tag.GetName().GetString() );
		DETHROW( deeInvalidParam );
	}
}

const char *deoalLSConfiguration::pGetCData( const decXmlElementTag &tag, const char *defaultValue ){
	decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData();
		
	}else{
		return defaultValue;
	}
}

bool deoalLSConfiguration::pGetCDataBool( const decXmlElementTag &tag, bool defaultValue ){
	const decString value( pGetCData( tag, defaultValue ? "true" : "false" ) );
	return value.EqualsInsensitive( "true" ) || value.EqualsInsensitive( "yes" ) || value == "1";
}

int deoalLSConfiguration::pGetCDataInt( const decXmlElementTag &tag, int defaultValue ){
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToInt();
		
	}else{
		return defaultValue;
	}
}

float deoalLSConfiguration::pGetCDataFloat( const decXmlElementTag &tag, float defaultValue ){
	const decXmlCharacterData * const cdata = tag.GetFirstData();
	
	if( cdata ){
		return cdata->GetData().ToFloat();
		
	}else{
		return defaultValue;
	}
}



void deoalLSConfiguration::pLoadConfig( decBaseFileReader *file ){
	decXmlDocumentReference xmlDoc;
	xmlDoc.TakeOver( new decXmlDocument );
	
	decXmlParser( pOal.GetGameEngine()->GetLogger() ).ParseXml( file, xmlDoc );
	
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	decXmlElementTag * const root = xmlDoc->GetRoot();
	if( ! root || strcmp( root->GetName(), "config" ) != 0 ){
		DETHROW( deeInvalidParam );
	}
	
	// read properties
	const int elementCount = root->GetElementCount();
	int i;
	
	for( i=0; i<elementCount; i++ ){
		const decXmlElementTag * const tag = pGetTagAt( *root, i );
		if( ! tag ){
			continue;
		}
		
		const decString &name = tag->GetName();
		
		if( name == "deviceName" ){
			pConfig.SetDeviceName( pGetCData( *tag, pConfig.GetDeviceName() ) );
			
		}else if( name == "enableEfx" ){
			pConfig.SetEnableEFX( pGetCDataBool( *tag, pConfig.GetEnableEFX() ) );
		
		
		
		}else if( name == "streamBufSizeThreshold" ){
			pConfig.SetStreamBufSizeThreshold( pGetCDataInt( *tag,
				pConfig.GetStreamBufSizeThreshold() ) );
			
		}else if( name == "disableExtension" ){
			pConfig.GetDisableExtensions().Add( pGetCData( *tag, "" ) );
			pConfig.SetDirty( true );
			
		}else if( name == "aurealizationMode" ){
			const decString value( pGetCData( *tag, "" ) );
			if( value == "disabled" ){
				pConfig.SetAurealizationMode( deoalConfiguration::eaDisabled );
				
			}else if( value == "directSound" ){
				pConfig.SetAurealizationMode( deoalConfiguration::eaDirectSound );
				
			}else if( value == "full" ){
				pConfig.SetAurealizationMode( deoalConfiguration::eaFull );
				
			}else{
				pOal.LogWarnFormat( "openal.xml %s(%i:%i): Unknown Value '%s'.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
			}
		
		}else if( name == "soundTraceRayCount" ){
			pConfig.SetSoundTraceRayCount( pGetCDataInt( *tag,
				pConfig.GetSoundTraceRayCount() ) );
		
		}else if( name == "soundTraceMaxBounceCount" ){
			pConfig.SetSoundTraceMaxBounceCount( pGetCDataInt( *tag,
				pConfig.GetSoundTraceMaxBounceCount() ) );
		
		}else if( name == "soundTraceMaxTransmitCount" ){
			pConfig.SetSoundTraceMaxTransmitCount( pGetCDataInt( *tag,
				pConfig.GetSoundTraceMaxTransmitCount() ) );
			
		}else if( name == "estimateRoomRayCount" ){
			pConfig.SetEstimateRoomRayCount( pGetCDataInt( *tag,
				pConfig.GetEstimateRoomRayCount() ) );
		
		
		
		}else if( name == "frameRateLimit" ){
			pConfig.SetFrameRateLimit( pGetCDataInt( *tag, pConfig.GetFrameRateLimit() ) );
			
		}else if( name == "asyncAudioSkipSyncTimeRatio" ){
			pConfig.SetAsyncAudioSkipSyncTimeRatio( pGetCDataFloat( *tag,
				pConfig.GetAsyncAudioSkipSyncTimeRatio() ) );
			
		}else if( name == "asyncAudio" ){
			pConfig.SetAsyncAudio( pGetCDataBool( *tag, pConfig.GetAsyncAudio() ) );
			
			
			
		}else{
			pOal.LogWarnFormat( "openal.xml %s(%i:%i): Unknown Tag %s.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}
