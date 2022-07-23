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

#include "deoglPSSAOMipMapBase.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOMipMapBase
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOMipMapBase::deoglPSSAOMipMapBase( deGraphicOpenGl &ogl ) : deoglParameterFloat( ogl ){
	SetName( "ssaoMipMapBase" );
	SetDescription( "Sets the mip map base for screen space ambient occlusion."
		" Screen space ambient occlusion is a distance based algorithm sampling in a"
		" certain radius around geometry. The projection of the sampling radius as"
		" defined by ssaoRadius onto the screen depends on the distance to the camera."
		" To enable using the same sampling count for all radi a downsampled version of"
		" the depth buffer is used. The choice of the mip map level to use determines"
		" the amount of noise, flickering and quality. The typical values are in the"
		" range from 4 to 16 pixels (2 raised to the power of 2-4). Smaller values"
		" cause higher mip map levels to be chosen which reduce noise but raises flickering."
		" Larger values cause lower mip map levels to be chosen which reduce flickering"
		" but raise noise. Higher noise can be compensated by increasing ssaoTapCount"
		" at the expense of higher runtime cost. The default value of 8 provides a"
		" reasonable balance between the two." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO MipMap Base" );
	SetDefaultValue( "8" );
}

deoglPSSAOMipMapBase::~deoglPSSAOMipMapBase(){
}



// Parameter Value
////////////////////

float deoglPSSAOMipMapBase::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOMipMapBase();
}

void deoglPSSAOMipMapBase::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOMipMapBase( value );
}
