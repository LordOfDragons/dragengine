/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoalPMaxEnvSlots.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPMaxEnvSlots
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPMaxEnvSlots::deoalPMaxEnvSlots( deAudioOpenAL &oal ) : deoalParameterInt( oal )
{
	SetName( "maxEnvs" );
	SetDescription( "Maximum count of environments to use for software auralization."
		" Larger values allow to simulate more different kinds of audio environments"
		" but is expensive. Smaller values runs faster but has less audio diversity."
		" If audio stutters or emits clicks try lowering this value."
		" This parameter is only used if 'AuralizationMode' is set to 'full'."
	);
	SetType( deModuleParameter::eptNumeric );
	SetMinimumValue( 2 );
	SetMaximumValue( 8 );
	
	SetCategory( ecBasic );
	SetDisplayName( "Maximum Environments" );
	SetDefaultValue( "8" );
}

deoalPMaxEnvSlots::~deoalPMaxEnvSlots(){
}



// Management
///////////////

int deoalPMaxEnvSlots::GetParameterInt(){
	return pOal.GetConfiguration().GetMaxSharedEffectSlots();
}

void deoalPMaxEnvSlots::SetParameterInt( int value ){
	pOal.GetConfiguration().SetMaxSharedEffectSlots( value );
}
