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

#include "deoalPAurealizationMode.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPAurealizationMode
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPAurealizationMode::deoalPAurealizationMode( deAudioOpenAL &oal ) : deoalParameter( oal )
{
	SetName( "aurealizationMode" );
	SetDescription( "Aurealization mode influeces how sound is calculate for the listener." );
	SetType( deModuleParameter::eptSelection );
	
	const deModuleParameter::SelectionEntry entries[ 3 ] = {
		{ "disabled", "Disabled", 
			"Aurealization disabled. Direct sound without any material based effects. "
			"This mode is enforced if APU has no EFX support or EFX support has been disabled." },
		{ "directSound", "Direct Sound",
			"Direct sound aurealization only. Direct sound effect based on materials are used. "
			"This includes muffling of sound over different frquency bands. Uses single collision "
			"test along sound direction from source to listener to calculate the result." },
		{ "full", "Full Aurealization",
			"Full aurealization. Enables all aurealization effects. Uses ray-tracing to calculate "
			"various parameters. This is expensive depending on the hardware used." }
	};
	
	AddSelectionEntries( entries, 3 );
	
	SetCategory( ecBasic );
	SetDisplayName( "Aurealization Mode" );
}

deoalPAurealizationMode::~deoalPAurealizationMode(){
}



// Management
///////////////

decString deoalPAurealizationMode::GetParameterValue(){
	switch( pOal.GetConfiguration().GetAurealizationMode() ){
	case deoalConfiguration::eaDisabled:
		return "disabled";
		
	case deoalConfiguration::eaDirectSound:
		return "directSound";
		
	case deoalConfiguration::eaFull:
		return "full";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deoalPAurealizationMode::SetParameterValue( const char *value ){
	const decString svalue( value );
	if( svalue.EqualsInsensitive( "disabled" ) ){
		pOal.GetConfiguration().SetAurealizationMode( deoalConfiguration::eaDisabled );
		
	}else if( svalue.EqualsInsensitive( "directSound" ) ){
		pOal.GetConfiguration().SetAurealizationMode( deoalConfiguration::eaDirectSound );
		
	}else if( svalue.EqualsInsensitive( "full" ) ){
		pOal.GetConfiguration().SetAurealizationMode( deoalConfiguration::eaFull );
		
	}else{
		pOal.GetConfiguration().SetAurealizationMode( deoalConfiguration::eaDisabled );
	}
}
