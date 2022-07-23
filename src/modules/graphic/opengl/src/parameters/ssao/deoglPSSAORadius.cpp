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

#include "deoglPSSAORadius.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAORadius
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAORadius::deoglPSSAORadius( deGraphicOpenGl &ogl ) : deoglParameterFloat( ogl ){
	SetName( "ssaoRadius" );
	SetDescription( "Sets the world space radius for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a"
		" certain radius around geometry."
		" The radius is set in meters and determines the distance after which a test ray"
		" is considered not occluded."
		" Typical values are in the range from 0.1m (1dm) to 1m."
		" Smaller values enhance small cravices missing though larger scale occlusion."
		" Large values provide a more correct larger scale ambient occlusion but have"
		" troubles with smaller cravices."
		" The risk of flickering, halos and edge undersampling rises with larger values."
		" The default value of 0.5 (5dm) provides a reasonable balance between the two." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Radius" );
	SetDefaultValue( "0.5" );
}

deoglPSSAORadius::~deoglPSSAORadius(){
}



// Parameter Value
////////////////////

float deoglPSSAORadius::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAORadius();
}

void deoglPSSAORadius::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAORadius( value );
}
