/* 
 * Drag[en]gine OpenGL Graphic Module
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
