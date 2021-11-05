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

#include "deoglPSSRCoveragePowerEdge.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoveragePowerEdge
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoveragePowerEdge::deoglPSSRCoveragePowerEdge( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrCoveragePowerEdge" );
	SetDescription( "Sets the power for edge coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Especially near the edge the calculation fails with higher probability."
		" Using edge coverage the found results near the screen edge are gradually faded out"
		" to avoid sharp differences between reflections just inside or outside the screen boundaries."
		" This parameter sets the power to which the edge coverage is raised."
		" The default is 2 which results in a quadratic fading from no coverage to full coverage."
		" A value of 1 results in a linear fading from no coverage to full coverage."
		" Values larger than 2 result in fading function using x-raisedto-N keeping the coverage longer"
		" near full coverage before falling down to no coverage."
		" This can be used to adjust the fading function to your personal liking." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Coverage Edge Power" );
	SetDefaultValue( "1" );
}

deoglPSSRCoveragePowerEdge::~deoglPSSRCoveragePowerEdge(){
}



// Parameter Value
////////////////////

float deoglPSSRCoveragePowerEdge::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoveragePowerEdge();
}

void deoglPSSRCoveragePowerEdge::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRCoveragePowerEdge( value );
}
