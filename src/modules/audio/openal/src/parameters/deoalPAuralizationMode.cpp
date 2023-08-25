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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoalPAuralizationMode.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPAuralizationMode
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPAuralizationMode::deoalPAuralizationMode( deAudioOpenAL &oal ) : deoalParameter( oal )
{
	SetName( "auralizationMode" );
	SetDescription( "Auralization mode influences how sound is calculate for the listener." );
	SetType( deModuleParameter::eptSelection );
	
	const deModuleParameter::SelectionEntry entries[ 3 ] = {
		{ "disabled", "Disabled", 
			"Auralization disabled. Direct sound without any material based effects. "
			"This mode is enforced if APU has no EFX support or EFX support has been disabled." },
		{ "directSound", "Direct Sound",
			"Direct sound auralization only. Direct sound effect based on materials are used. "
			"This includes muffling of sound over different frquency bands. Uses single collision "
			"test along sound direction from source to listener to calculate the result." },
		{ "full", "Full auralization",
			"Full auralization. Enables all auralization effects. Uses ray-tracing to calculate "
			"various parameters. This is expensive depending on the hardware used." }
	};
	
	AddSelectionEntries( entries, 3 );
	
	SetCategory( ecBasic );
	SetDisplayName( "Auralization Mode" );
	SetDefaultValue( "full" );
}

deoalPAuralizationMode::~deoalPAuralizationMode(){
}



// Management
///////////////

decString deoalPAuralizationMode::GetParameterValue(){
	switch( pOal.GetConfiguration().GetAuralizationMode() ){
	case deoalConfiguration::eamDisabled:
		return "disabled";
		
	case deoalConfiguration::eamDirectSound:
		return "directSound";
		
	case deoalConfiguration::eamFull:
		return "full";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deoalPAuralizationMode::SetParameterValue( const char *value ){
	const decString svalue( decString( value ).GetLower() );
	if( svalue.EqualsInsensitive( "disabled" ) ){
		pOal.GetConfiguration().SetAuralizationMode( deoalConfiguration::eamDisabled );
		
	}else if( svalue.EqualsInsensitive( "directsound" ) ){
		pOal.GetConfiguration().SetAuralizationMode( deoalConfiguration::eamDirectSound );
		
	}else if( svalue.EqualsInsensitive( "full" ) ){
		pOal.GetConfiguration().SetAuralizationMode( deoalConfiguration::eamFull );
		
	}else{
		pOal.GetConfiguration().SetAuralizationMode( deoalConfiguration::eamDisabled );
	}
}
