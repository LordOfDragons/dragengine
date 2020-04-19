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

#include "deoglPSSRCoverageEdgeSize.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRCoverageEdgeSize
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRCoverageEdgeSize::deoglPSSRCoverageEdgeSize( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssrCoverageEdgeSize" );
	SetDescription( "Sets the edge size for edge coverage calculation in screen space reflection."
		" Many reflections can not be calculated properly in screen space."
		" Especially near the edge the calculation fails with higher probability."
		" Using edge coverage the found results near the screen edge are gradually faded out"
		" to avoid sharp differences between reflections just inside or outside the screen boundaries."
		" This parameter sets the fading distance from the edge as percentage of half the screen dimension."
		" The default value is 0.1 (10%) which equals to 5% edge of the screen dimension."
		" Higher values enlarge the fading distance which might be more visually more pleasant."
		" Speed wise this has no impact." );
	SetCategory( ecExpert );
	SetDisplayName( "SSR Coverage Edge Size" );
}

deoglPSSRCoverageEdgeSize::~deoglPSSRCoverageEdgeSize(){
}



// Parameter Value
////////////////////

float deoglPSSRCoverageEdgeSize::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSRCoverageEdgeSize();
}

void deoglPSSRCoverageEdgeSize::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSRCoverageEdgeSize( value );
}
