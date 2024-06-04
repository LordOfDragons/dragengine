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

#include "deoglPSSRStepCount.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRStepCount
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRStepCount::deoglPSSRStepCount( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "ssrStepCount" );
	SetDescription( "Count of steps used in SSR calculation. "
		" Influences the overall SSR quality versus render performance." );
	SetType( deModuleParameter::eptSelection );
	SetDisplayName( "SSR Step Count" );
	SetCategory( ecBasic );
	
	const deModuleParameter::SelectionEntry entries[ 5 ] = {
		{ "veryLow", "Very Low", "Very low step count." },
		{ "low", "Low", "Low step count." },
		{ "medium", "Medium", "Medium step count." },
		{ "high", "High", "High step count." },
		{ "veryHigh", "Very High", "Very high step count." }
	};
	AddSelectionEntries( entries, 5 );
	
	SetDefaultValue( "low" );
}

deoglPSSRStepCount::~deoglPSSRStepCount(){
}



// Parameter Value
////////////////////

decString deoglPSSRStepCount::GetParameterValue(){
	const int count = pOgl.GetConfiguration().GetSSRStepCount();
	
	if( count <= 10 ){
		return "veryLow";
		
	}else if( count <= 20 ){
		return "low";
		
	}else if( count <= 40 ){
		return "medium";
		
	}else if( count <= 80 ){
		return "high";
		
	}else{
		return "veryHigh";
	}
}

void deoglPSSRStepCount::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "verylow" ){
		pOgl.GetConfiguration().SetSSRStepCount( 10 );
		
	}else if( checkValue == "low" ){
		pOgl.GetConfiguration().SetSSRStepCount( 20 );
		
	}else if( checkValue == "medium" ){
		pOgl.GetConfiguration().SetSSRStepCount( 40 );
		
	}else if( checkValue == "high" ){
		pOgl.GetConfiguration().SetSSRStepCount( 80 );
		
	}else if( checkValue == "veryhigh" ){
		pOgl.GetConfiguration().SetSSRStepCount( 160 );
		
	}else{
		pOgl.GetConfiguration().SetSSRStepCount( 40 );
	}
}
