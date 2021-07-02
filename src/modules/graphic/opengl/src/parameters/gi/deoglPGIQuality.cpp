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

#include "deoglPGIQuality.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// class deoglPGIQuality
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglPGIQuality::deoglPGIQuality( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "giQuality" );
	SetDescription( "Global Illumination Quality. Higher quality uses higher count of traced "
		"rays to improve lighting quality. Lower quality uses lower count of traced rays "
		"improving performance." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "GI Quality" );
	
	const deModuleParameter::SelectionEntry entries[ 6 ] = {
		{ "off", "Off", "Disable Global Illumination. Uses light ambient intensities."
			" This can break games so use this option only if nothing else helps." },
		{ "veryLow", "Very Low", "Very low quality. For weak GPU delivering best performance." },
		{ "low", "Low", "Low quality. Prefer performance over quality." },
		{ "medium", "Medium", "Medium quality. Balance between quality and performance. Recommended choice." },
		{ "high", "High", "High quality. Prefer quality over performance." },
		{ "veryHigh", "Very High", "Very high quality. For high-end GPU delivering best quality." }
	};
	AddSelectionEntries( entries, 6 );
}

deoglPGIQuality::~deoglPGIQuality(){
}



// Parameter Value
////////////////////

decString deoglPGIQuality::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetGIQuality() ){
	case deoglConfiguration::egiqOff:
		return "off";
		
	case deoglConfiguration::egiqVeryLow:
		return "veryLow";
		
	case deoglConfiguration::egiqLow:
		return "low";
		
	case deoglConfiguration::egiqMedium:
		return "medium";
		
	case deoglConfiguration::egiqHigh:
		return "high";
		
	case deoglConfiguration::egiqVeryHigh:
		return "veryHigh";
		
	default:
		return "high";
	}
}

void deoglPGIQuality::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "off" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqOff );
		
	}else if( checkValue == "verylow" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqVeryLow );
		
	}else if( checkValue == "low" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqLow );
		
	}else if( checkValue == "medium" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqMedium );
		
	}else if( checkValue == "high" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqHigh );
		
	}else if( checkValue == "veryhigh" ){
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqVeryHigh );
		
	}else{
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqMedium );
	}
}
