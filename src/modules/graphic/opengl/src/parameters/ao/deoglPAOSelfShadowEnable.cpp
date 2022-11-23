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

#include "deoglPAOSelfShadowEnable.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPAOSelfShadowEnable
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPAOSelfShadowEnable::deoglPAOSelfShadowEnable( deGraphicOpenGl &ogl ) :
deoglParameterBool( ogl )
{
	SetName( "aoSelfShadowEnable" );
	SetDescription( "Enables Ambient Occlusion (AO) self shadowing."
		" This adds more details to shadows near object surface." );
	SetCategory( ecBasic );
	SetDisplayName( "AO Self Shadow" );
	SetDefaultValue( "1" );
}

deoglPAOSelfShadowEnable::~deoglPAOSelfShadowEnable(){
}



// Parameter Value
////////////////////

bool deoglPAOSelfShadowEnable::GetParameterBool(){
	return pOgl.GetConfiguration().GetAOSelfShadowEnable();
}

void deoglPAOSelfShadowEnable::SetParameterBool( bool value ){
	pOgl.GetConfiguration().SetAOSelfShadowEnable( value );
}
