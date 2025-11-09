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
		
		alcGetIntegerv( device, ALC_HRTF_SOFT, 1, &result );
		logger.LogInfoFormat( "- HRTF Enabled = %d", result == ALC_TRUE );
		
		alcGetIntegerv( device, ALC_HRTF_STATUS_SOFT, 1, &result );
		switch( result ){
		case ALC_HRTF_DISABLED_SOFT:
			logger.LogInfo( "- HRTF Status: Disabled" );
			break;
			
		case ALC_HRTF_ENABLED_SOFT:
			logger.LogInfo( "- HRTF Status: Enabled" );
			break;
			
		case ALC_HRTF_DENIED_SOFT:
			logger.LogInfo( "- HRTF Status: Denied" );
			break;
			
		case ALC_HRTF_REQUIRED_SOFT:
			logger.LogInfo( "- HRTF Status: Required" );
			break;
			
		case ALC_HRTF_HEADPHONES_DETECTED_SOFT:
			logger.LogInfo( "- HRTF Status: Enabled, headphones detected" );
			break;
			
		case ALC_HRTF_UNSUPPORTED_FORMAT_SOFT:
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
		
		alcGetIntegerv( device, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, ( ALCint* )&count );
		for( i=0; i<count; i++ ){
			pHRTFSpecifiers.Add( palcGetStringi( device, ALC_HRTF_SPECIFIER_SOFT, i ) );
		}
	}
}
