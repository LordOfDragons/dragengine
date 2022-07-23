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

#include "deoglPAOSelfShadowSmoothAngle.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPAOSelfShadowSmoothAngle
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPAOSelfShadowSmoothAngle::deoglPAOSelfShadowSmoothAngle( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "aoSelfShadowSmoothAngle" );
	SetDescription( "Sets the ambient occlusion self shadowing smoothing angle."
		" The self shadowing algorithm is by nature an on-off test."
		" The smoothing angle avoids the harsh step from shadows to unshadows areas by"
		" defining an angle range in degrees over which the shadow fades."
		" Typical values are in the range from 5 to 20 degrees."
		" The default value is 6 degrees."
		" The minimal value is 0.1 and the maximal value 90."
		" Values outside the range from 5 to 20 are though not recommended.\n"
		"\n"
		" 5 works best for small features like texture ambient occlusion on flat surfaces."
		" 15 works best for large features like curved and normal mapped geometry with"
		" SSAO but likes to fail due to not-oriented cone."
		" thus for SSAO self-shadowing is disabled using only texture ambient occlusion."
		" 6 seems to be a good value for this case." );
	SetCategory( ecExpert );
	SetDisplayName( "AO Self Shadow Smooth Angle" );
	SetDefaultValue( "10" );
}

deoglPAOSelfShadowSmoothAngle::~deoglPAOSelfShadowSmoothAngle(){
}



// Parameter Value
////////////////////

float deoglPAOSelfShadowSmoothAngle::GetParameterFloat(){
	return pOgl.GetConfiguration().GetAOSelfShadowSmoothAngle();
}

void deoglPAOSelfShadowSmoothAngle::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetAOSelfShadowSmoothAngle( value );
}
