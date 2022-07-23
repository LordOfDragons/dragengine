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

#include "deoglPSSAOTapCount.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOTapCount
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOTapCount::deoglPSSAOTapCount( deGraphicOpenGl &ogl ) :
deoglParameterInt( ogl )
{
	SetName( "ssaoTapCount" );
	SetDescription( "Sets the number of samples to take for screen space ambient occlusion."
		" This determines the overall quality of the screen space ambient occlusion versus"
		" rendering speed. With lower tap counts the calculated ambient occlusion becomes"
		" more noisy. A value of 9 or lower should be used on weaker systems where rendering"
		" speed is more important than visual quality. A value between 9 to 18 gives good"
		" quality. Values above 18 do not improve quality considerably.  18 is the"
		" recommended value for modern graphic cards." );
	SetCategory( ecExpert );
	SetDisplayName( "SSAO Tap Count" );
	SetDefaultValue( "18" );
}

deoglPSSAOTapCount::~deoglPSSAOTapCount(){
}



// Parameter Value
////////////////////

int deoglPSSAOTapCount::GetParameterInt(){
	return pOgl.GetConfiguration().GetSSAOTapCount();
}

void deoglPSSAOTapCount::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetSSAOTapCount( value );
}
