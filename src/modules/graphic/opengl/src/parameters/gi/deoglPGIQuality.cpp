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
	SetDefaultValue( "high" );
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
		pOgl.GetConfiguration().SetGIQuality( deoglConfiguration::egiqHigh );
	}
}
