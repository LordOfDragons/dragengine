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

#include "deoalAudioThread.h"
#include "deoalATContext.h"
#include "deoalATLogger.h"
#include "../deoalBasics.h"
#include "../capabilities/deoalCapabilities.h"
#include "../component/deoalComponent.h"
#include "../extensions/deoalExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoalATContext
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalATContext::deoalATContext( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pDevice( NULL ),
pContext( NULL ){
}

deoalATContext::~deoalATContext(){
}



// Management
///////////////

void deoalATContext::OpenDevice(){
	const decString &deviceName = pAudioThread.GetConfiguration().GetDeviceName();
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	pScanForDevices();
	
	if( deviceName.IsEmpty() ){
		logger.LogInfo( "Open default device" );
		pDevice = alcOpenDevice( NULL );
		
	}else{
		logger.LogInfoFormat( "Open device '%s' instead of default", deviceName.GetString() );
		pDevice = alcOpenDevice( deviceName );
	}
	
	if( ! pDevice ){
		logger.LogErrorFormat( "alcOpenDevice(%s) failed", deviceName.GetString() );
		DETHROW( deeInvalidParam );
	}
}

void deoalATContext::CreateContext(){
	// create context. the attributes are hints. after creating it should be possible
	// to query the actually supported values. ALSoft seems to accept all values without
	// using them anyhow.
	deoalExtensions &extensions = pAudioThread.GetExtensions();
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	int index = 0;
	ALCint attributes[ 9 ];
	
	attributes[ index++ ] = ALC_MONO_SOURCES;
	attributes[ index++ ] = 1024 * 8;
	
	attributes[ index++ ] = ALC_STEREO_SOURCES;
	attributes[ index++ ] = 1024 * 8;
	
	if( extensions.GetHasEFX() ){
		attributes[ index++ ] = ALC_MAX_AUXILIARY_SENDS;
		attributes[ index++ ] = 8;
			// NOTE a send can have both an effect and a filter. to do environment simulation
			//      it is enough to have a reverb effect and a low-pass filter. thus 1 send
			//      is enough to simulate 1 indirect sound path
	}
	
	if( extensions.GetHasHRTF() ){
		attributes[ index++ ] = ALC_HRTF;
		attributes[ index++ ] = ALC_DONT_CARE;
			// let driver choose when to use HRTF. we can force it with ALC_TRUE but then
			// it is possible the user is using a surround system where HRTF is not good.
			// we check later on though if HRTF is currently in use.
	}
	
	attributes[ index++ ] = ALC_INVALID;
	
	pContext = alcCreateContext( pDevice, attributes );
	if( ! pContext ){
		logger.LogError( "alcCreateContext failed" );
		DETHROW( deeInvalidParam );
	}
	
	// make the context current
	if( alcMakeContextCurrent( pContext ) == AL_FALSE ){
		logger.LogError( "alcMakeContextCurrent failed" );
		DETHROW( deeInvalidParam );
	}
	
	// set default parameters
	alDistanceModel( AL_INVERSE_DISTANCE_CLAMPED );
}

void deoalATContext::LogContextInfo(){
	deoalATLogger &logger = pAudioThread.GetLogger();
	ALint frequency;
	ALint refresh;
	
	OAL_CHECK( pAudioThread, alcGetIntegerv( pDevice, ALC_FREQUENCY, 1, &frequency ) );
	OAL_CHECK( pAudioThread, alcGetIntegerv( pDevice, ALC_REFRESH, 1, &refresh ) );
	
	logger.LogInfoFormat( "Context: Mixing Frequency = %i Hz", frequency );
	logger.LogInfoFormat( "Context: Refresh Interval = %i Hz", refresh );
}

void deoalATContext::CleanUp(){
	if( pContext ){
		alcMakeContextCurrent( NULL );
		alcDestroyContext( pContext );
		pContext = NULL;
	}
	
	if( pDevice ){
		alcCloseDevice( pDevice );
		pDevice = NULL;
	}
}



// Private Functions
//////////////////////

void deoalATContext::pScanForDevices(){
	const ALCchar *defaultDevice = NULL;
	const ALCchar *devices = NULL;
	int position, len;
	
	// query for the informations
	defaultDevice = alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
	
	if( alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) == AL_TRUE ){
		defaultDevice = alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
		devices = alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );
		
	}else if( alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" ) == AL_TRUE ){
		devices = alcGetString( NULL, ALC_DEVICE_SPECIFIER );
	}
	
	// log devices
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	if( devices ) {
		logger.LogInfo( "devices:" );
		position = 0;
		while( devices[ position ] ){
			len = strlen( devices + position );
			if( len > 0 ){
				logger.LogInfoFormat( "- %s", devices + position );
				position += len + 1;
			}
		}
		
	}else{
		logger.LogError( "devices(all): < query failed >" );
	}
	
	logger.LogInfoFormat( "default device: '%s'", defaultDevice ? defaultDevice : "< query failed >" );
}
