/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
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
	
	decBaseFileReader::Ref reader(decBaseFileReader::Ref::New( vfs.OpenFileForReading( path ) ));
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
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
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
			
		}else if( name == "AuralizationMode" ){
			const decString value( pGetCData( *tag, "" ) );
			
			if( value == "disabled" ){
				pConfig.SetAuralizationMode( deoalConfiguration::eamDisabled );
				
			}else if( value == "directSound" ){
				pConfig.SetAuralizationMode( deoalConfiguration::eamDirectSound );
				
			}else if( value == "full" ){
				pConfig.SetAuralizationMode( deoalConfiguration::eamFull );
				
			}else{
				pOal.LogWarnFormat( "openal.xml %s(%i:%i): Unknown Value '%s'.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
			}
			
		}else if( name == "AuralizationQuality" ){
			const decString value( pGetCData( *tag, "" ) );
			
			if( value == "veryLow" ){
				pConfig.SetAuralizationQuality( deoalConfiguration::eaqVeryLow );
				
			}else if( value == "low" ){
				pConfig.SetAuralizationQuality( deoalConfiguration::eaqLow );
				
			}else if( value == "medium" ){
				pConfig.SetAuralizationQuality( deoalConfiguration::eaqMedium );
				
			}else if( value == "high" ){
				pConfig.SetAuralizationQuality( deoalConfiguration::eaqHigh );
				
			}else if( value == "veryHigh" ){
				pConfig.SetAuralizationQuality( deoalConfiguration::eaqVeryHigh );
				
			}else{
				pOal.LogWarnFormat( "openal.xml %s(%i:%i): Unknown Value '%s'.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
			}
			
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
		
		
		
		}else if( name == "useSharedEffectSlots" ){
			pConfig.SetUseSharedEffectSlots( pGetCDataBool(
				*tag, pConfig.GetUseSharedEffectSlots() ) );
		
		}else if( name == "shareEnvironmentThreshold" ){
			pConfig.SetShareEnvironmentThreshold( pGetCDataFloat(
				*tag, pConfig.GetShareEnvironmentThreshold() ) );
		
		}else if( name == "switchSharedEnvironmentThreshold" ){
			pConfig.SetSwitchSharedEnvironmentThreshold( pGetCDataFloat(
				*tag, pConfig.GetSwitchSharedEnvironmentThreshold() ) );
		
		}else if( name == "maxSharedEffectSlots" ){
			pConfig.SetMaxSharedEffectSlots( pGetCDataInt(
				*tag, pConfig.GetMaxSharedEffectSlots() ) );
			
			
			
		}else{
			pOal.LogWarnFormat( "openal.xml %s(%i:%i): Unknown Tag %s.",
				root->GetName().GetString(), tag->GetLineNumber(),
				tag->GetPositionNumber(), tag->GetName().GetString() );
		}
	}
}
