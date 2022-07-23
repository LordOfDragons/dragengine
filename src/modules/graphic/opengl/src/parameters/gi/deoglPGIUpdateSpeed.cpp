/* 
 * Drag[en]gine OpenGL Graphic Module
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
