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

#include "deoglPSSAORadiusLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAORadiusLimit
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAORadiusLimit::deoglPSSAORadiusLimit( deGraphicOpenGl &ogl ) : deoglParameterFloat( ogl ){
	SetName( "ssaoRadiusLimit" );
	SetDescription( "Sets the screen space radius limit for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a certain"
		" radius around geometry. The projection of the sampling radius as defined by"
		" ssaoRadius onto the screen depends on the distance to the camera. With decreasing"
		" distance the screen space radius quickly grows resulting in a sub-optimal size."
		" This parameter limits the radius for close geometry to a percentage of the screen"
		" size to avoid problems. The default value of 0.5 (50% of the screen size) provides"
		" a reasonable limit. This is an advanced parameter and should be left at the default." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Radius Limit" );
	SetDefaultValue( "0.5" );
}

deoglPSSAORadiusLimit::~deoglPSSAORadiusLimit(){
}



// Parameter Value
////////////////////

float deoglPSSAORadiusLimit::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAORadiusLimit();
}

void deoglPSSAORadiusLimit::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAORadiusLimit( value );
}
