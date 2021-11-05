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
#include "deoglPLightCutOffIntensity.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "dragengine/common/exceptions.h"



// class deoglPLightCutOffIntensity
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPLightCutOffIntensity::deoglPLightCutOffIntensity( deGraphicOpenGl &ogl ) : deoglParameterFloat( ogl ){
	SetName( "lightCutOffIntensity" );
	SetDescription( "Sets the cut off intensity for light sources."
		" Lights are set to 0 beyond the distance where the intensity drops below this intensity." );
	SetCategory( ecExpert );
	SetDisplayName( "Light Cut-Off Intensity" );
	SetDefaultValue( "0.01" );
}

deoglPLightCutOffIntensity::~deoglPLightCutOffIntensity(){
}



// Parameter Value
////////////////////

float deoglPLightCutOffIntensity::GetParameterFloat(){
	return pOgl.GetConfiguration().GetLightCutOffIntensity();
}

void deoglPLightCutOffIntensity::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetLightCutOffIntensity( value );
}
