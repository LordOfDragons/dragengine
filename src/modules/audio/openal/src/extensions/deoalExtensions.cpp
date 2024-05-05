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
}

deoalExtensions::~deoalExtensions(){
}



// Management
///////////////

void deoalExtensions::ScanDeviceExtensions() {
	ALCdevice * const device = pAudioThread.GetContext().GetDevice();
	
	const char * const strExtensions = ( const char * )alcGetString( device, ALC_EXTENSIONS );
	if( strExtensions ){
		pStrListExtensions = decString( strExtensions ).Split( ' ' );
	}
	
	int i;
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = pStrListExtensions.Has( vExtensionNames[ i ] );
	}
	
	pDisableExtensions();
	
	pHasEFX = pHasExtension[ ext_ALC_EXT_EFX ];
	pHasHRTF = pHasExtension[ ext_ALC_SOFT_HRTF ];
}

void deoalExtensions::ScanContextExtensions(){
	const char * const strExtensions = ( const char * )alGetString( AL_EXTENSIONS );
	if( strExtensions ){
		pStrListExtensions += decString( strExtensions ).Split( ' ' );
	}
	
	pStrListExtensions.SortAscending();
	
	deoalATLogger &logger = pAudioThread.GetLogger();
	logger.LogInfo( "Extensions:" );
	const int count = pStrListExtensions.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		logger.LogInfoFormat( "- %s", pStrListExtensions.GetAt( i ).GetString() );
	}
	
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = pStrListExtensions.Has( vExtensionNames[ i ] );
	}
	
	pDisableExtensions();
	
	pFetchRequiredFunctions();
	pFetchOptionalFunctions();
}

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
		
		if( ! pHasExtension[ ext_ALC_EXT_EFX ] ){
			DETHROW( deeInvalidAction );
		}
	}
	
	// ALC_SOFT_HRTF
	if( pHasExtension[ ext_ALC_SOFT_HRTF ] ){
		pGetOptionalFunction( (void**)&palcGetStringi, "alcGetStringiSOFT", ext_ALC_SOFT_HRTF );
		pGetOptionalFunction( (void**)&palcResetDevice, "alcResetDeviceSOFT", ext_ALC_SOFT_HRTF );
		
		if( ! pHasExtension[ ext_ALC_SOFT_HRTF ] ){
			DETHROW( deeInvalidAction );
		}
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
