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

#include "deoalFilter.h"
#include "../audiothread/deoalAudioThread.h"
#include "../extensions/deoalExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoalFilter
//////////////////////

// Constructor, destructor
////////////////////////////

deoalFilter::deoalFilter( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pType( eftNull ),
pGain( 1.0f ),
pGainLF( 1.0f ),
pGainHF( 1.0f ),
pFilter( 0 )
{
	if( ! audioThread.GetExtensions().GetHasEFX() ){
		DETHROW( deeInvalidParam );
	}
	
	OAL_CHECK( audioThread, palGenFilters( 1, &pFilter ) );
}

deoalFilter::~deoalFilter(){
	if( pFilter ){
		palDeleteFilters( 1, &pFilter );
	}
}



// Management
///////////////

void deoalFilter::SetType( eFilterType type ){
	if( type == pType ){
		return;
	}
	
	pType = type;
	
	switch( type ){
	case eftNull:
		OAL_CHECK( pAudioThread, palFilteri( pFilter, AL_FILTER_TYPE, AL_FILTER_NULL ) );
		break;
		
	case eftLowPass:
		OAL_CHECK( pAudioThread, palFilteri( pFilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS ) );
		break;
		
	case eftHighPass:
		OAL_CHECK( pAudioThread, palFilteri( pFilter, AL_FILTER_TYPE, AL_FILTER_HIGHPASS ) );
		break;
		
	case eftBandPass:
		OAL_CHECK( pAudioThread, palFilteri( pFilter, AL_FILTER_TYPE, AL_FILTER_BANDPASS ) );
		break;
	}
}

void deoalFilter::SetGain( float gain ){
	pGain = gain;
	
	switch( pType ){
	case eftLowPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_LOWPASS_GAIN, gain ) );
		break;
		
	case eftHighPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_HIGHPASS_GAIN, gain ) );
		break;
		
	case eftBandPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAIN, gain ) );
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGainLF( float gain ){
	pGainLF = gain;
	
	switch( pType ){
	case eftHighPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_HIGHPASS_GAINLF, gain ) );
		break;
		
	case eftBandPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAINLF, gain ) );
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGainHF( float gain ){
	pGainHF = gain;
	
	switch( pType ){
	case eftLowPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_LOWPASS_GAINHF, gain ) );
		break;
		
	case eftBandPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAINHF, gain ) );
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGains( float gain, float gainLF, float gainHF ){
	pGain = gain;
	pGainLF = gainLF;
	pGainHF = gainHF;
	
	switch( pType ){
	case eftLowPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_LOWPASS_GAIN, pGain ) );
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_LOWPASS_GAINHF, pGainHF ) );
		break;
		
	case eftHighPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_HIGHPASS_GAIN, pGain ) );
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_HIGHPASS_GAINLF, pGainLF ) );
		break;
		
	case eftBandPass:
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAIN, pGain ) );
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAINLF, pGainLF ) );
		OAL_CHECK( pAudioThread, palFilterf( pFilter, AL_BANDPASS_GAINHF, pGainHF ) );
		break;
		
	default:
		break;
	}
}
