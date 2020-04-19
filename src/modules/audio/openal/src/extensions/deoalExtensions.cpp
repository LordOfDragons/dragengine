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

#include "deoalExtensions.h"
#include "../deoalBasics.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATContext.h"

/*
#ifdef ANDROID
extern __eglMustCastToProperFunctionPointerType androidGetProcAddress( const char *name );
#endif
*/

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

static const char * const vExtensionNames[ deoalExtensions::EXT_COUNT ] = {
	"ALC_EXT_EFX",
	"ALC_SOFT_HRTF",
};



// Class deoalExtensions
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalExtensions::deoalExtensions( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pVersionMajor( 0 ),
pVersionMinor( 0 ),
pHasRequiredFunctions( true ),
pEfxVersionMajor( 0 ),
pEfxVersionMinor( 0 )
{
	int i;
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = false;
		pDisableExtension[ i ] = false;
	}
	
	pScanVersion();
	pScanExtensions();
	pDisableExtensions();
	
	pFetchRequiredFunctions();
	pFetchOptionalFunctions();
	
	pHasEFX = pHasExtension[ ext_ALC_EXT_EFX ];
	pHasHRTF = pHasExtension[ ext_ALC_SOFT_HRTF ];
}

deoalExtensions::~deoalExtensions(){
}



// Management
///////////////

void deoalExtensions::PrintSummary(){
	deoalATLogger &logger = pAudioThread.GetLogger();
	int i;
	
	logger.LogInfoFormat( "Version: %d.%d", pVersionMajor, pVersionMinor );
	
	logger.LogInfo( "Supported Extensions:" );
	for( i=0; i<EXT_COUNT; i++ ){
		if( pHasExtension[ i ] ){
			logger.LogInfoFormat( "- %s", vExtensionNames[ i ] );
		}
	}
	
	logger.LogInfo( "Not Supported Extensions:" );
	for( i=0; i<EXT_COUNT; i++ ){
		if( ! pHasExtension[ i ] ){
			logger.LogInfoFormat( "- %s", vExtensionNames[ i ] );
		}
	}
	
	if( pHasExtension[ ext_ALC_EXT_EFX ] ){
		logger.LogInfoFormat( "EFX Version: %d.%d", pEfxVersionMajor, pEfxVersionMinor );
	}
}

bool deoalExtensions::VerifyPresence(){
	return pHasRequiredFunctions;
}

bool deoalExtensions::GetHasExtension( eExtensions extension ) const{
	return pHasExtension[ extension ];
}

const char *deoalExtensions::GetExtensionName( eExtensions extension ) const{
	return vExtensionNames[ extension ];
}

void deoalExtensions::DisableExtension( eExtensions extension ){
	pHasExtension[ extension ] = false;
	pDisableExtension[ extension ] = true;
}



// Private functions
//////////////////////

void deoalExtensions::pScanVersion(){
	ALCdevice * const device = pAudioThread.GetContext().GetDevice();
	alcGetIntegerv( device, ALC_MAJOR_VERSION, 1, ( ALint* )&pVersionMajor );
	alcGetIntegerv( device, ALC_MINOR_VERSION, 1, ( ALint* )&pVersionMinor );
}

void deoalExtensions::pScanExtensions(){
	ALCdevice * const device = pAudioThread.GetContext().GetDevice();
	const char *strExtensions = ( const char * )alcGetString( device, ALC_EXTENSIONS );
	if( ! strExtensions ){
		strExtensions = "";
	}
	
	/*
	logger.LogInfoFormat( "pScanExtensions: '%s'", strExtensions);
	- ALC_ENUMERATE_ALL_EXT
	- ALC_ENUMERATION_EXT
	- ALC_EXT_CAPTURE
	- ALC_EXT_DEDICATED
	- ALC_EXT_disconnect
	- ALC_EXT_EFX
	- ALC_EXT_thread_local_context
	- ALC_SOFTX_device_clock
	- ALC_SOFT_HRTF
	- ALC_SOFT_loopback
	- ALC_SOFT_output_limiter
	- ALC_SOFT_pause_device
	*/
	
	pStrListExtensions = decString( strExtensions ).Split( ' ' );
	pStrListExtensions.SortAscending();
	
	int i;
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = pStrListExtensions.Has( vExtensionNames[ i ] );
	}
}

void deoalExtensions::pDisableExtensions(){
	const decStringSet &list = pAudioThread.GetConfiguration().GetDisableExtensions();
	deoalATLogger &logger = pAudioThread.GetLogger();
	const int count = list.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		const decString &name = list.GetAt( i );
		
		for( j=0; j<EXT_COUNT; j++ ){
			if( name == vExtensionNames[ j ] ){
				logger.LogInfoFormat( "Disable extension '%s'", name.GetString() );
				DisableExtension( ( eExtensions )j );
			}
		}
	}
}

void deoalExtensions::pFetchRequiredFunctions(){
}

void deoalExtensions::pFetchOptionalFunctions(){
	ALCdevice * const device = pAudioThread.GetContext().GetDevice();
	
	// ALC_EXT_EFX
	if( pHasExtension[ ext_ALC_EXT_EFX ] ){
		alcGetIntegerv( device, ALC_EFX_MAJOR_VERSION, 1, ( ALint* )&pEfxVersionMajor );
		alcGetIntegerv( device, ALC_EFX_MINOR_VERSION, 1, ( ALint* )&pEfxVersionMinor );
		
		pGetOptionalFunction( (void**)&palGenEffects, "alGenEffects", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palDeleteEffects, "alDeleteEffects", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palEffecti, "alEffecti", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palEffectiv, "alEffectiv", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palEffectf, "alEffectf", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palEffectfv, "alEffectfv", ext_ALC_EXT_EFX );
		
		pGetOptionalFunction( (void**)&palGenFilters, "alGenFilters", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palDeleteFilters, "alDeleteFilters", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palFilteri, "alFilteri", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palFilteriv, "alFilteriv", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palFilterf, "alFilterf", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palFilterfv, "alFilterfv", ext_ALC_EXT_EFX );
		
		pGetOptionalFunction( (void**)&palGenAuxiliaryEffectSlots, "alGenAuxiliaryEffectSlots", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palDeleteAuxiliaryEffectSlots, "alDeleteAuxiliaryEffectSlots", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palAuxiliaryEffectSloti, "alAuxiliaryEffectSloti", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palAuxiliaryEffectSlotiv, "alAuxiliaryEffectSlotiv", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palAuxiliaryEffectSlotf, "alAuxiliaryEffectSlotf", ext_ALC_EXT_EFX );
		pGetOptionalFunction( (void**)&palAuxiliaryEffectSlotfv, "alAuxiliaryEffectSlotfv", ext_ALC_EXT_EFX );
	}
	
	// ALC_SOFT_HRTF
	if( pHasExtension[ ext_ALC_SOFT_HRTF ] ){
		pGetOptionalFunction( (void**)&palcGetStringi, "alcGetStringiSOFT", ext_ALC_SOFT_HRTF );
		pGetOptionalFunction( (void**)&palcResetDevice, "alcResetDeviceSOFT", ext_ALC_SOFT_HRTF );
	}
}



void deoalExtensions::pGetRequiredFunction( void **funcPointer, const char *funcName ){
	void * const fp = alcGetProcAddress( pAudioThread.GetContext().GetDevice(), funcName );
	
	if( ! fp ){
		pAudioThread.GetLogger().LogErrorFormat(
			"Failed to get a suitable function address for %s", funcName );
		pHasRequiredFunctions = false;
	}
	
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoalExtensions::pGetOptionalFunction( void **funcPointer, const char *funcName, int extensionIndex ){
	void * const fp = alcGetProcAddress( pAudioThread.GetContext().GetDevice(), funcName );
	
	if( ! fp ){
		pAudioThread.GetLogger().LogErrorFormat(
			"Failed to get a suitable function address for %s although extension "
			"%s is supported. This is a driver bug!", funcName, vExtensionNames[ extensionIndex ] );
		pHasExtension[ extensionIndex ] = false;
	}
	
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}
