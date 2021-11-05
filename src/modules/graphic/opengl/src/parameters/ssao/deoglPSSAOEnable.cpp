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

#include "deoglPSSAOEnable.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSAOEnable
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSAOEnable::deoglPSSAOEnable( deGraphicOpenGl &ogl ) : deoglParameterBool( ogl ){
	SetName( "ssaoEnable" );
	SetDescription( "Sets if screen space ambient occlusion is enabled."
		" Screen space ambient occlusion calculates a dynamic ambient occlusion using the"
		" rendered depth image to determine the degree of visibility of geometry on screen"
		" relative to nearby geometry."
		" This works though only for information present in the rendered depth image."
		" Ambient occlusion is used to improve the lighting especially for shadow areas."
		" The used screen space ambient occlusion algorithm is rather fast to calculate"
		" while producing reasonably good results."
		" The screen space ambient occlusion works together with the ambient.occlusion"
		" related texture properties."
		" To fine tune the screen space ambient occlusion see the ssao* module parameters." );
	SetCategory( ecBasic );
	SetDisplayName( "SSAO" );
	SetDefaultValue( "1" );
}

deoglPSSAOEnable::~deoglPSSAOEnable(){
}



// Parameter Value
////////////////////

bool deoglPSSAOEnable::GetParameterBool(){
	return pOgl.GetConfiguration().GetSSAOEnable();
}

void deoglPSSAOEnable::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetSSAOEnable( value );
}
