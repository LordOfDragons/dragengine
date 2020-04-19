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

#include "deoglPSSAOSelfOcclusionAngle.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOSelfOcclusionAngle
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOSelfOcclusionAngle::deoglPSSAOSelfOcclusionAngle( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssaoSelfOcclusionAngle" );
	SetDescription( "Sets the self occlusion angle for screen space ambient occlusion."
		" Due to limited accuracy of the rendered depth image self occlusion can happen"
		" on flat or nearly flat geometry. To avoid this effect geometry is not taken"
		" into account for nearby occlusion if the angle is less than an threshold."
		" The threshold is defined as angle in degrees relative to the geometry."
		" Typical values are in the range from 10 to 30 degrees."
		" Small values reduce the risk of missing nearby geometry at shallow angles but"
		" runs the risk of false self occlusion in curved geometry."
		" Large values reduce the chance of false self occlusion but run the risk of"
		" missing geometry at shallow angles."
		" Shallow angles can happen near corners and affect mostly small cravices."
		" The default value is 10."
		" Increase if self occlusion becomes a problem." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Self Occlusion Angle" );
}

deoglPSSAOSelfOcclusionAngle::~deoglPSSAOSelfOcclusionAngle(){
}



// Parameter Value
////////////////////

float deoglPSSAOSelfOcclusionAngle::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOSelfOcclusionAngle();
}

void deoglPSSAOSelfOcclusionAngle::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOSelfOcclusionAngle( value );
}
