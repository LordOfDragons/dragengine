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

#include "deoglPGIUpdateSpeed.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// class deoglPGIUpdateSpeed
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPGIUpdateSpeed::deoglPGIUpdateSpeed( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "giUpdateSpeed" );
	SetDescription( "Global Illumination Update Speed. Higher update speed converges lighting "
		"faster decreasing reaction time to changes in light conditions. Lower update speed "
		"converges lighting slower improving performance." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "GI Update Speed" );
	
	const deModuleParameter::SelectionEntry entries[ 6 ] = {
		{ "veryLow", "Very Low", "Very low update speed. For weak GPU delivering best performance." },
		{ "low", "Low", "Low update speed. Prefer performance over faster convergence." },
		{ "medium", "Medium", "Medium update speed. Balance between convergence speed "
			"and performance. Recommended choice." },
		{ "high", "High", "High update speed. Prefer faster convergence over performance." },
		{ "veryHigh", "Very High", "Very high update speed. For high-end GPU delivering fastest convergence." }
	};
	AddSelectionEntries( entries, 6 );
	SetDefaultValue( "medium" );
}

deoglPGIUpdateSpeed::~deoglPGIUpdateSpeed(){
}



// Parameter Value
////////////////////

decString deoglPGIUpdateSpeed::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetGIUpdateSpeed() ){
	case deoglConfiguration::egiusVeryLow:
		return "veryLow";
		
	case deoglConfiguration::egiusLow:
		return "low";
		
	case deoglConfiguration::egiusMedium:
		return "medium";
		
	case deoglConfiguration::egiusHigh:
		return "high";
		
	case deoglConfiguration::egiusVeryHigh:
		return "veryHigh";
		
	default:
		return "medium";
	}
}

void deoglPGIUpdateSpeed::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "verylow" ){
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusVeryLow );
		
	}else if( checkValue == "low" ){
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusLow );
		
	}else if( checkValue == "medium" ){
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusMedium );
		
	}else if( checkValue == "high" ){
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusHigh );
		
	}else if( checkValue == "veryhigh" ){
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusVeryHigh );
		
	}else{
		pOgl.GetConfiguration().SetGIUpdateSpeed( deoglConfiguration::egiusMedium );
	}
}
