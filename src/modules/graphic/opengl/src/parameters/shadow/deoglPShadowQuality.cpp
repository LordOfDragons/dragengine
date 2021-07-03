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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglPShadowQuality.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "dragengine/common/exceptions.h"



// class deoglPShadowQuality
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPShadowQuality::deoglPShadowQuality( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "shadowQuality" );
	SetDescription( "Quality of shadow maps. Higher quality uses higher shadow map size "
		"to improve shadow quality. Lower quality uses lower shadow map size improving performance." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "Shadow Quality" );
	
	const deModuleParameter::SelectionEntry entries[ 6 ] = {
		{ "off", "Off", "Disable shadow mapping. This can break games so use this option "
			"only if nothing else helps." },
		{ "veryLow", "Very Low", "Very low quality. For weak GPU delivering best performance." },
		{ "low", "Low", "Low quality. Prefer performance over quality." },
		{ "medium", "Medium", "Medium quality. Balance between quality and performance. Recommended choice." },
		{ "high", "High", "High quality. Prefer quality over performance." },
		{ "veryHigh", "Very High", "Very high quality. For high-end GPU delivering best quality." }
	};
	AddSelectionEntries( entries, 5 );
}

deoglPShadowQuality::~deoglPShadowQuality(){
}



// Parameter Value
////////////////////

decString deoglPShadowQuality::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetShadowQuality() ){
	case deoglConfiguration::esqOff:
		return "off";
		
	case deoglConfiguration::esqVeryLow:
		return "veryLow";
		
	case deoglConfiguration::esqLow:
		return "low";
		
	case deoglConfiguration::esqMedium:
		return "medium";
		
	case deoglConfiguration::esqHigh:
		return "high";
		
	case deoglConfiguration::esqVeryHigh:
		return "veryHigh";
		
	default:
		return "high";
	}
}

void deoglPShadowQuality::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "off" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqOff );
		
	}else if( checkValue == "verylow" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqVeryLow );
		
	}else if( checkValue == "low" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqLow );
		
	}else if( checkValue == "medium" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqMedium );
		
	}else if( checkValue == "high" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqHigh );
		
	}else if( checkValue == "veryhigh" ){
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqVeryHigh );
		
	}else{
		pOgl.GetConfiguration().SetShadowQuality( deoglConfiguration::esqHigh );
	}
}
