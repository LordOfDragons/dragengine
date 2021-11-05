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

#include "deoglPSSAOEdgeBlurThreshold.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOEdgeBlurThreshold
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOEdgeBlurThreshold::deoglPSSAOEdgeBlurThreshold( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssaoEdgeBlurThreshold" );
	SetDescription( "Sets the depth threshold for the blur pass of the screen space ambient occlusion."
		" Screen space ambient occlusion is a stochastic algorithm introducing noise into the result "
		" due to taking less samples than required to cover the entire sampling range for spead reason."
		" To hide the noise a blur pass is applied to the calculated ambient occlusion."
		" This blur is an edge aware blur using depth differences to avoid bluring across edges."
		" This parameter defines the depth threshold between neighbor pixels to fade out to blur towards."
		" Smaller values increase the blur hiding noise better but wash out edges in the distance"
		" which can cause light bleeding."
		" Larger values reduce the blur on the noise resulting in a sharper image but with more"
		" noise especially in the distance."
		" The default value is 5 which equals a depth difference of 0.2m at 1m distance from the camera." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Edge Blur Threshold" );
	SetDefaultValue( "5" );
}

deoglPSSAOEdgeBlurThreshold::~deoglPSSAOEdgeBlurThreshold(){
}



// Parameter Value
////////////////////

float deoglPSSAOEdgeBlurThreshold::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOEdgeBlurThreshold();
}

void deoglPSSAOEdgeBlurThreshold::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOEdgeBlurThreshold( value );
}
