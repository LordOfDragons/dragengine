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

#include "deoalCapabilities.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATContext.h"
#include "../extensions/deoalExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoalCapabilities
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalCapabilities::deoalCapabilities( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pMaxMonoSources( 0 ),
pMaxStereoSources( 0 ),
pEfxMaxAuxSend( 0 )
{
	pDetectCapabilities();
}

deoalCapabilities::~deoalCapabilities(){
}



// Management
///////////////

void deoalCapabilities::ReportCapabilities(){
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	logger.LogInfo( "Capabilities:" );
	logger.LogInfoFormat( "- Maximum Mono Sources = %d", pMaxMonoSources );
	logger.LogInfoFormat( "- Maximum Stereo Sources = %d", pMaxStereoSources );
	logger.LogInfoFormat( "- EFX Maximum AUX Send = %d", pEfxMaxAuxSend );
	
	if( pAudioThread.GetExtensions().GetHasHRTF() ){
		ALCdevice * const device = pAudioThread.GetContext().GetDevice();
		ALCint result;
		
		logger.LogInfo( "- HRTF Specifiers:" );
		const int specifierCount = pHRTFSpecifiers.GetCount();
		int i;
		for( i=0; i<specifierCount; i++ ){
			logger.LogInfoFormat( "  - %s", pHRTFSpecifiers.GetAt( i ).GetString() );
		}
		
		alcGetIntegerv( device, ALC_HRTF, 1, &result );
		logger.LogInfoFormat( "- HRTF Enabled = %d", result == ALC_TRUE );
		
		alcGetIntegerv( device, ALC_HRTF_STATUS, 1, &result );
		switch( result ){
		case ALC_HRTF_DISABLED:
			logger.LogInfo( "- HRTF Status: Disabled" );
			break;
			
		case ALC_HRTF_ENABLED:
			logger.LogInfo( "- HRTF Status: Enabled" );
			break;
			
		case ALC_HRTF_DENIED:
			logger.LogInfo( "- HRTF Status: Denied" );
			break;
			
		case ALC_HRTF_REQUIRED:
			logger.LogInfo( "- HRTF Status: Required" );
			break;
			
		case ALC_HRTF_HEADPHONES_DETECTED:
			logger.LogInfo( "- HRTF Status: Enabled, headphones detected" );
			break;
			
		case ALC_HRTF_UNSUPPORTED_FORMAT:
			logger.LogInfo( "- HRTF Status: Disabled, unsupported format" );
			break;
			
		default:
			logger.LogInfoFormat( "- HRTF Status: Unknown enumeration 0x%x", result );
		}
	}
}



// Private Functions
//////////////////////

void deoalCapabilities::pDetectCapabilities(){
	deoalExtensions &extensions = pAudioThread.GetExtensions();
	ALCdevice * const device = pAudioThread.GetContext().GetDevice();
	
	alcGetIntegerv( device, ALC_MONO_SOURCES, 1, ( ALint* )&pMaxMonoSources );
	alcGetIntegerv( device, ALC_STEREO_SOURCES, 1, ( ALint* )&pMaxStereoSources );
	
	if( extensions.GetHasEFX() ){
		alcGetIntegerv( device, ALC_MAX_AUXILIARY_SENDS, 1, ( ALint* )&pEfxMaxAuxSend );
	}
	
	if( pAudioThread.GetExtensions().GetHasHRTF() ){
		int count, i;
		
		alcGetIntegerv( device, ALC_NUM_HRTF_SPECIFIERS, 1, ( ALCint* )&count );
		for( i=0; i<count; i++ ){
			pHRTFSpecifiers.Add( palcGetStringi( device, ALC_HRTF_SPECIFIER, i ) );
		}
	}
}
