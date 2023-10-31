/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoalPAuralizationQuality.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPAuralizationQuality
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPAuralizationQuality::deoalPAuralizationQuality( deAudioOpenAL &oal ) : deoalParameter( oal )
{
	SetName( "auralizationQuality" );
	SetDescription( "Auralization quality weighting quality versus performance."
		" This parameter is only used if 'AuralizationMode' is set to 'full'."
	);
	SetType( deModuleParameter::eptSelection );
	
	const deModuleParameter::SelectionEntry entries[ 5 ] = {
		{ "veryLow", "Very Low", "Very low quality. For weak CPU delivering best performance." },
		{ "low", "Low", "Low quality. Prefer performance over quality." },
		{ "medium", "Medium", "Medium quality. Balance between quality and performance. Recommended choice." },
		{ "high", "High", "High quality. Prefer quality over performance." },
		{ "veryHigh", "Very High", "Very high quality. For high-end CPU delivering best quality." }
	};
	
	AddSelectionEntries( entries, 5 );
	
	SetCategory( ecBasic );
	SetDisplayName( "Auralization Quality" );
	SetDefaultValue( "medium" );
}

deoalPAuralizationQuality::~deoalPAuralizationQuality(){
}



// Management
///////////////

decString deoalPAuralizationQuality::GetParameterValue(){
	switch( pOal.GetConfiguration().GetAuralizationQuality() ){
	case deoalConfiguration::eaqVeryLow:
		return "veryLow";
		
	case deoalConfiguration::eaqLow:
		return "low";
		
	case deoalConfiguration::eaqMedium:
		return "medium";
		
	case deoalConfiguration::eaqHigh:
		return "high";
		
	case deoalConfiguration::eaqVeryHigh:
		return "veryHigh";
		
	default:
		return "medium";
	}
}

void deoalPAuralizationQuality::SetParameterValue( const char *value ){
	const decString svalue( decString( value ).GetLower() );
	if( svalue.EqualsInsensitive( "verylow" ) ){
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqVeryLow );
		
	}else if( svalue.EqualsInsensitive( "low" ) ){
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqLow );
		
	}else if( svalue.EqualsInsensitive( "medium" ) ){
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqMedium );
		
	}else if( svalue.EqualsInsensitive( "high" ) ){
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqHigh );
		
	}else if( svalue.EqualsInsensitive( "veryhigh" ) ){
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqVeryHigh );
		
	}else{
		pOal.GetConfiguration().SetAuralizationQuality( deoalConfiguration::eaqMedium );
	}
}
