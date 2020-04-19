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

#include "deoglPSSAOTurnCount.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOTurnCount
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOTurnCount::deoglPSSAOTurnCount( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "ssaoTurnCount" );
	SetDescription( "Sets the number of spiral turns to take for screen space ambient occlusion."
		" Screen space ambient occlusion is a stochastic algorithm introducing noise into the result "
		" due to taking less samples than required to cover the entire sampling range for spead reason."
		" This requires distributing samples across the sampling range in an optimal pattern."
		" The algorithm uses a sort of spiral pattern to distribute the sampling points."
		" This parameter determines the number of turns to take for this spiral."
		" Values should be prime numbers to avoid visible patterns."
		" This is an advanced parameter that is best left to the default value of 7 turns." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Turn Count" );
}

deoglPSSAOTurnCount::~deoglPSSAOTurnCount(){
}



// Parameter Value
////////////////////

float deoglPSSAOTurnCount::GetParameterFloat(){
	return pOgl.GetConfiguration().GetSSAOTurnCount();
}

void deoglPSSAOTurnCount::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetSSAOTurnCount( value );
}
