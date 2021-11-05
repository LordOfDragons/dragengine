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

#include "deoglPSSRMaxRayLength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRMaxRayLength
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRMaxRayLength::deoglPSSRMaxRayLength( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrMaxRayLength" );
	SetDescription( "Sets the maximum length of reflection rays in screen space reflections"
		" as percentage of the screen size. The largest screen size (width or height) is"
		" multiplied with this factor to determine the maximum ray length in pixels."
		" Limiting the maximum ray length may improve the ray sampling quality without"
		" impacting rendering speed. A value of 0.5 limits the ray length to at most 50%"
		" of the screen thus sampling step wide is smaller compared to the ray at 100%"
		" length with the same ssrStepCount. As downside this reduces though the distance"
		" of local reflections. This is more an esthetic tuning value to balance local"
		" reflections versus global reflections. For rendering speed improvements better"
		" use the ssrStepCount parameter." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Max Ray Length" );
	SetDefaultValue( "1" );
}

deoglPSSRMaxRayLength::~deoglPSSRMaxRayLength(){
}



// Parameter Value
////////////////////

float deoglPSSRMaxRayLength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRMaxRayLength();
}

void deoglPSSRMaxRayLength::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRMaxRayLength( value );
}
