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

#include "deoglPSSRCoveragePowerRayLength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoveragePowerRayLength
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoveragePowerRayLength::deoglPSSRCoveragePowerRayLength( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrCoveragePowerRayLength" );
	SetDescription( "Sets the power for ray length coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Using ray length coverage the found results are faded out gradually the longer a ray is until it hits geometry."
		" This parameter sets the power to which the ray length coverage is raised."
		" A value of 1 results in a linear fading from no coverage to full coverage."
		" A value of 2 results in a quadratic fading from no coverage to full coverage."
		" Values larger than 2 result in fading function using x-raisedto-N keeping the coverage longer"
		" near full coverage before falling down to no coverage."
		" The default value is 8 which results fading out results rather close to the maximum ray length."
		" This can be used to adjust the fading function to your personal liking." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Coverage Power Ray Length" );
}

deoglPSSRCoveragePowerRayLength::~deoglPSSRCoveragePowerRayLength(){
}



// Parameter Value
////////////////////

float deoglPSSRCoveragePowerRayLength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoveragePowerRayLength();
}

void deoglPSSRCoveragePowerRayLength::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRCoveragePowerRayLength( value );
}
