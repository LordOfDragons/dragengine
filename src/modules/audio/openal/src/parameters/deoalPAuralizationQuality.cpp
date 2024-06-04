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
